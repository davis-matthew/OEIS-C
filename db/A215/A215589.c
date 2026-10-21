/*
 * A215589  Value of x in the n-th number of the form x+y*(1+sqrt(2)).
 * https://oeis.org/A215589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1064, u16 n <= 1064, u32 n <= 1064, u64 n <= 1064,
 *            u128 n <= 1064, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the numbers  x + y*(1+√2) in increasing order by
 * maintaining one monotone sequence for each fixed y.  Each sequence
 * is sorted, and we keep a min‑heap of the current smallest element from
 * each sequence.  When the smallest is extracted we return the x
 * part (x = m – y) and then push the next element of the same sequence.
 * The comparison of two values
 *     v1 = m1 + k1√2 ,  v2 = m2 + k2√2
 * is done without floating point by comparing the sign of
 *     (m1-m2) + (k1-k2)√2
 * using only integer arithmetic.  This guarantees exact ordering for
 * all arguments that fit in seq_t.  Complexity: O(n log n) time,
 * O(n) memory for n up to 10 000. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

struct Node { idx_t m; idx_t k; };

/* return -1 if a<b, 1 if a>b, 0 if equal */
static int node_cmp(const struct Node *a, const struct Node *b)
{
    __int128 p = (__int128)a->m - (__int128)b->m; /* a.m - b.m */
    __int128 q = (__int128)a->k - (__int128)b->k; /* a.k - b.k */

    if (q == 0) {
        if (p < 0) return -1;
        if (p > 0) return  1;
        return 0;
    }

    if (q > 0) {                /* compare p + q√2 with 0, q > 0 */
        if (p < 0) {
            __int128 lhs = p * p;                    /* (negative)²  */
            __int128 rhs = (__int128)2 * q * q;      /* 2*q²        */
            return (lhs > rhs) ? -1 : 1;             /* a<b ? */
        }
        return 1;                                    /* positive sum */
    } else {                /* q < 0, i.e. compare p - |q|√2 with 0 */
        if (p < 0) return -1;                         /* negative sum */
        /* p >= 0 */
        __int128 lhs = p * p;
        __int128 rhs = (__int128)2 * (-q) * (-q);
        return (lhs < rhs) ? -1 : 1;
    }
}

/* sift down heap[0..size-1] from index i */
static void sift_down(struct Node *heap, size_t size, size_t i)
{
    for (;;) {
        size_t left  = 2 * i + 1;
        size_t right = left + 1;
        size_t smallest = i;

        if (left < size && node_cmp(&heap[left], &heap[smallest]) < 0)
            smallest = left;
        if (right < size && node_cmp(&heap[right], &heap[smallest]) < 0)
            smallest = right;

        if (smallest == i) break;
        struct Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

/* build heap from array of size n */
static void heapify(struct Node *heap, size_t n)
{
    if (n < 2) return;
    for (size_t i = (n - 2) / 2 + 1; i-- > 0; ) sift_down(heap, n, i);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safety, although offset is 1 */

    /* Allocate one node for each y=0…n-1 */
    size_t size = (size_t)n;
    struct Node *heap = (struct Node*)malloc(size * sizeof(struct Node));
    if (!heap) return (seq_t)0;          /* out of memory guard */

    /* initialise nodes: for y=k, smallest m is k (m = x + y, x ≥ 0) */
    for (size_t i = 0; i < size; ++i) {
        heap[i].k = (idx_t)i;
        heap[i].m = (idx_t)(i == 0 ? 0 : i);
    }
    heapify(heap, size);

    for (idx_t cnt = 1; cnt <= n; ++cnt) {
        struct Node cur = heap[0];
        if (cnt == n) {
            /* x = m – k  */
            seq_t result = (seq_t)(cur.m - cur.k);
            free(heap);
            return result;
        }
        /* replace root with next element of the same y  */
        heap[0].m = cur.m + 1;            /* same k, m increased by 1 */
        sift_down(heap, size, 0);
    }
    free(heap);                            /* unreachable, keeps compiler happy */
    return (seq_t)0;
}
