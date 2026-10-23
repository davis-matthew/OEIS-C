/*
 * A023116  Signature sequence of 1/sqrt(3) (arrange the numbers i+j*x (i,j >=
 *          1) in increasing order; the sequence of i's is the signature of
 *          x).
 * https://oeis.org/A023116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   Compute the signature sequence of x = 1/√3 by merging the infinite set
   of values i + j/x (i,j ≥ 1).  A min‑heap of pairs (i,j) is maintained.
   At each step the smallest value is extracted; its i‑coordinate is part
   of the sequence.  After extraction the pair (i,j+1) is re‑inserted and
   a new pair (k,1) is inserted for the next unused integer k.  The
   comparison of two pairs is done without floating point by comparing
   the integer quantities
          (i1 - i2) * S + (j1 - j2) * SCALE
   where S = floor(√3 · 2⁶⁴) and SCALE = 2⁶⁴.  S is a 128‑bit constant
   computed once.  The algorithm runs in O(n log n), fast enough for
   the required limits. */

#include <stdint.h>
#include <stdlib.h>

/* External integer types – do not redeclare them. */
typedef uint64_t idx_t;

/* Constant scales for integer comparison.  S = floor(√3 · 2⁶⁴).  */
static const unsigned __int128 SCALE  = ((unsigned __int128)1) << 64;
static const unsigned __int128 SCONST = (((unsigned __int128)1) << 64) + (unsigned long long)13491405884753487518ULL;
/*  SCONST ≈ 31938149958463039134  ≈ √3 · 2⁶⁴ */

/* Pair of indices (i,j).  */
typedef struct { idx_t i; idx_t j; } pair_t;

/* Compare two pairs: returns true if p1 < p2 in the order of i + j/x. */
static int pair_less(const pair_t *p1, const pair_t *p2)
{
    __int128 di = (__int128)p1->i - (__int128)p2->i;
    __int128 dj = (__int128)p1->j - (__int128)p2->j;
    __int128 diff = di * (__int128)SCONST + dj * (__int128)SCALE;
    return diff < 0;
}

/* Swap two heap elements. */
static void heap_swap(pair_t *a, pair_t *b)
{
    pair_t t = *a; *a = *b; *b = t;
}

/* Restore heap property upward from index idx. */
static void sift_up(pair_t *heap, size_t idx, size_t size)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (!pair_less(&heap[idx], &heap[parent]))
            break;
        heap_swap(&heap[idx], &heap[parent]);
        idx = parent;
    }
}

/* Restore heap property downward from index idx. */
static void sift_down(pair_t *heap, size_t idx, size_t size)
{
    while (1) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left < size  && pair_less(&heap[left],  &heap[smallest]))
            smallest = left;
        if (right < size && pair_less(&heap[right], &heap[smallest]))
            smallest = right;

        if (smallest == idx)
            break;
        heap_swap(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

/* Return the a(n)-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined, return 0 */

    /* Allocate space for up to 2n+1 heap elements to avoid reallocation. */
    size_t max_size = (size_t)n * 2 + 1;
    pair_t *heap = (pair_t *)malloc(sizeof(pair_t) * max_size);
    if (heap == NULL)
        return (seq_t)0;      /* allocation failure */

    size_t heap_sz = 0;
    idx_t next_new_i = 2;    /* next i to insert into heap */

    /* Seed heap with first pair (1,1). */
    heap[heap_sz++] = (pair_t){1, 1};

    for (idx_t k = 1; k <= n; ++k) {
        /* Extract min. */
        pair_t min = heap[0];
        heap_sz--;
        if (heap_sz > 0) {
            heap[0] = heap[heap_sz];
            sift_down(heap, 0, heap_sz);
        }

        if (k == n) {              /* last term required */
            free(heap);
            return (seq_t)min.i;
        }

        /* Re‑insert the next j for the same i. */
        if (heap_sz < max_size) {
            heap[heap_sz++] = (pair_t){min.i, min.j + 1};
            sift_up(heap, heap_sz - 1, heap_sz);
        }

        /* Insert a new i if not already inserted. */
        if (next_new_i <= n && heap_sz < max_size) {
            heap[heap_sz++] = (pair_t){next_new_i, 1};
            sift_up(heap, heap_sz - 1, heap_sz);
            next_new_i++;
        }
    }

    free(heap);   /* should never reach here */
    return (seq_t)0;
}
