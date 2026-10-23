/*
 * A138670  Indices of 0's in Stewart's choral sequence.
 * https://oeis.org/A138670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 5155, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A138670: Indices of 0's in Stewart's choral sequence 
   Formula: the set {k*3^(m+1) + (3^m-1)/2 with k ≥ 0 and m ≥ 0}.
   The sequence is the sorted list of these numbers.

   This implementation generates the needed terms by merging an
   infinite family of arithmetic progressions with a binary min‑heap.
 */

#include <stddef.h>

typedef struct {
    seq_t val;   /* current term in the progression */
    idx_t m;     /* progression index m */
} HeapItem;

/* binary min‑heap helpers --------------------------------------------- */
static void heap_push(HeapItem heap[], size_t *sz, HeapItem it)
{
    size_t i = (*sz)++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= it.val) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = it;
}

static HeapItem heap_pop(HeapItem heap[], size_t *sz)
{
    HeapItem ret = heap[0];
    size_t i = 0, last = --(*sz);
    HeapItem tmp = heap[last];
    while (1) {
        size_t l = (i << 1) + 1;
        if (l > last) break;
        size_t r = l + 1;
        size_t best = l;
        if (r <= last && heap[r].val < heap[l].val) best = r;
        if (tmp.val <= heap[best].val) break;
        heap[i] = heap[best];
        i = best;
    }
    heap[i] = tmp;
    return ret;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* maximum number of progressions needed for n up to a few million
       is far below 512; 512 is safely large for 128‑bit arithmetic. */
    #define MAX_M 512

    /* pre‑compute 3^i for i = 0 … MAX_M+1 */
    static seq_t pow3[MAX_M + 2];
    pow3[0] = 1;
    for (idx_t i = 1; i <= (idx_t)(MAX_M + 1); ++i)
        pow3[i] = pow3[i - 1] * 3;

    /* heap that holds the current minimal term from each progression */
    HeapItem heap[MAX_M + 2];
    size_t heap_sz = 0;

    /* next progression index to consider for insertion into the heap */
    idx_t next_m = 0;

    /* helper that inserts all progressions whose starting term is
       less than or equal to the current heap minimum. */
    while (next_m <= (idx_t)MAX_M) {
        seq_t start = (pow3[next_m] - 1) / 2;          /* (3^m-1)/2 */
        if (heap_sz > 0 && start > heap[0].val) break; /* stop inserting */
        heap_push(heap, &heap_sz, (HeapItem){ start, next_m });
        ++next_m;
    }

    size_t k = 0;  /* number of terms already output */
    for (;;) {
        /* ensure that no progression with a starting term smaller
           than the heap minimum remains uninserted */
        while (next_m <= (idx_t)MAX_M) {
            seq_t start = (pow3[next_m] - 1) / 2;
            if (heap_sz > 0 && start > heap[0].val) break;
            heap_push(heap, &heap_sz, (HeapItem){ start, next_m });
            ++next_m;
        }

        HeapItem cur = heap_pop(heap, &heap_sz);  /* smallest unprocessed term */

        if (k == n) return cur.val;              /* desired element found */

        /* push the next term from the same progression */
        seq_t next_val = cur.val + pow3[cur.m + 1];    /* add the common difference */
        heap_push(heap, &heap_sz, (HeapItem){ next_val, cur.m });

        ++k;
    }
}
