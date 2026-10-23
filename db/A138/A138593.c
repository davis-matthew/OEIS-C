/*
 * A138593  Sums of 5 or more consecutive nonnegative integers.
 * https://oeis.org/A138593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1315, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that can be written as the sum of at least 5 consecutive non‑negative
 * integers.  They are of the form s*k + k*(k-1)/2 with k ≥ 5, s ≥ 0. */

#include <stdlib.h>
#include <stdbool.h>

/* Each node represents one arithmetic progression with difference k =
 * the length of the consecutive block.  val holds the current term in the
 * progression. */
typedef struct {
    seq_t val;   /* current value of the progression */
    idx_t k;     /* block length (k ≥ 5) */
} node_t;

/* Simple binary min‑heap on the val field. */
static void sift_down(node_t *h, size_t sz, size_t i)
{
    while (1) {
        size_t l = 2 * i + 1;
        if (l >= sz) break;
        size_t r = l + 1;
        size_t smallest = l;
        if (r < sz && h[r].val < h[l].val) smallest = r;
        if (h[smallest].val < h[i].val) {
            node_t tmp = h[i];
            h[i] = h[smallest];
            h[smallest] = tmp;
            i = smallest;
        } else break;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, safeguard */

    /* Number of progressions to initialise.  Using n+5 comfortably covers
     * the first n terms for all n ≤ 10000. */
    idx_t Kmax = n + 5;
    if (Kmax < 5) Kmax = 5;

    /* Allocate heap.  Size_t is sufficient for the small sizes used. */
    node_t *heap = (node_t *)malloc((size_t)Kmax * sizeof(node_t));
    if (!heap) return 0;  /* allocation failure */

    /* Initialise heap: first term of each progression k=5..Kmax+4 is
     * k*(k-1)/2. */
    for (idx_t i = 0; i < Kmax; ++i) {
        idx_t k = i + 5;
        heap[i].k = k;
        heap[i].val = ((seq_t)k * (seq_t)(k - 1)) / 2;
    }

    /* Build min‑heap. */
    for (size_t i = Kmax / 2; i > 0; --i)
        sift_down(heap, (size_t)Kmax, i - 1);

    seq_t last = 0;
    bool first = true;
    idx_t count = 0;

    while (1) {
        node_t *top = &heap[0];
        if (first || top->val != last) {
            ++count;
            if (count == n) {
                seq_t result = top->val;
                free(heap);
                return result;
            }
            last = top->val;
            first = false;
        }
        /* Advance the progression represented by the root. */
        top->val += top->k;
        sift_down(heap, (size_t)Kmax, 0);
    }
}
