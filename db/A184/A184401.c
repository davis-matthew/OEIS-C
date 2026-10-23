/*
 * A184401  Ranks of (even i)+(odd j)*r, when all i+j*r are ranked; r=sqrt(2),
 *          i>=0, j>=0.
 * https://oeis.org/A184401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A184401
   a(n) = rank of the nth pair (i,j) with i even and j odd
   when all pairs (i,j), i>=0, j>=0, are sorted by
   the value i + j*sqrt(2).  The implementation enumerates
   the sorted order with a binary min‑heap, using a 128‑bit
   scaled integer to avoid floating point arithmetic. */

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    idx_t   i;
    idx_t   j;
    __int128 val;          /* scaled value = i*SCALE + j*SQRTSCALE */
} Node;

/* heap helper: sift down a node at position pos */
static void sift_down(Node* heap, size_t heap_size, size_t pos)
{
    for (;; ) {
        size_t left = (pos << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && heap[right].val < heap[left].val)
            smallest = right;
        if (heap[smallest].val < heap[pos].val) {
            Node tmp = heap[pos];
            heap[pos] = heap[smallest];
            heap[smallest] = tmp;
            pos = smallest;
        } else {
            break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not defined, but safe */

    /* scaling constants */
    const uint64_t SCALE = 1ULL << 63;                     /* 2^63  */
    const uint64_t SQRT2_NUM = 1414213562373095048ULL;      /* sqrt(2)*1e18  */
    const uint64_t SQRT2_DEN = 1000000000000000000ULL;      /* 1e18          */
    const uint64_t SQRT2_SCALED = (uint64_t)(((__int128)SQRT2_NUM * (__int128)SCALE) / SQRT2_DEN);

    /* each j contributes a chain of increasing i */
    idx_t jmax = n * 3;          /* j up to ≈ 3n is sufficient (≈ 2.8n would do) */
    size_t heap_size = (size_t)jmax + 1;

    Node *heap = (Node*)malloc(heap_size * sizeof(Node));
    if (!heap) return 0;          /* allocation failure – unlikely in tests */

    /* initialise heap with i=0 for every j */
    for (idx_t j = 0; j <= jmax; ++j) {
        heap[j].i   = 0;
        heap[j].j   = j;
        heap[j].val = (__int128)SQRT2_SCALED * j;   /* i = 0 */
    }

    /* build min‑heap */
    for (long idx = (long)heap_size/2 - 1; idx >= 0; --idx)
        sift_down(heap, heap_size, (size_t)idx);

    idx_t rank   = 0;          /* current rank in full ordering */
    idx_t count  = 0;          /* number of (even i, odd j) seen so far */
    seq_t result = 0;

    while (count < n) {
        Node cur = heap[0];    /* node with minimal scaled value */
        ++rank;

        if (((cur.i & 1ULL) == 0) && ((cur.j & 1ULL) == 1)) {
            ++count;
            if (count == n) {
                result = rank;    /* n‑th desired pair reached */
                break;
            }
        }

        /* advance this chain: increase i by 1 */
        cur.i   += 1;
        cur.val += SCALE;     /* add 1 * SCALE to the scaled value */
        heap[0] = cur;
        sift_down(heap, heap_size, 0);
    }

    free(heap);
    return result;
}
