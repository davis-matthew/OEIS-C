/*
 * A104599  Dimensions of the irreducible representations of the simple Lie
 *          algebra of type G2 over the complex numbers, listed in increasing
 *          order.
 * https://oeis.org/A104599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th smallest dimension of an irreducible
 *            representation of the complex simple Lie algebra of type G₂. The
 *            Weyl dimension formula for a highest weight λ = a ω₁ + b ω₂ (a,b
 *            ≥ 0) is dim(a,b) = ((a+1)(b+1)(a+b+2)(a+2b+3)(a+3b+4)(2a+3b+5))
 *            / 120 The sequence lists all distinct values of d...
 * Range:     u8 none, u16 none, u32 none, u64 n <= 9521, u128 n <= 9521,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_seq(const void *p, const void *q)
{
    seq_t x = *(const seq_t *)p;
    seq_t y = *(const seq_t *)q;
    return (x > y) - (x < y);
}

seq_t a(idx_t n)
{
    /* Static cache of the computed sequence */
    static seq_t *seq   = NULL;
    static idx_t seq_len = 0;

    if (seq_len == 0) {
        /* choose a bound that yields many terms (≈360 000 pairs) */
        const idx_t maxA = 600;                 /* (maxA+1)^2 ≈ 3.6·10⁵  */
        idx_t pair_count = (maxA + 1) * (maxA + 1);

        /* temporary array for all dimensions before deduplication */
        seq_t *tmp = (seq_t *)malloc(pair_count * sizeof(seq_t));
        idx_t k = 0;

        for (idx_t a = 0; a <= maxA; ++a) {
            for (idx_t b = 0; b <= maxA; ++b) {
                unsigned __int128 p = 1;
                p *= (unsigned __int128)(a + 1);
                p *= (unsigned __int128)(b + 1);
                p *= (unsigned __int128)(a + b + 2);
                p *= (unsigned __int128)(a + 2 * b + 3);
                p *= (unsigned __int128)(a + 3 * b + 4);
                p *= (unsigned __int128)(2 * a + 3 * b + 5);
                p /= 120;
                tmp[k++] = (seq_t)p;
            }
        }

        /* sort the list */
        qsort(tmp, k, sizeof(seq_t), cmp_seq);

        /* deduplicate */
        seq_len = 1;
        seq     = (seq_t *)malloc(k * sizeof(seq_t));
        seq[0]  = tmp[0];
        for (idx_t i = 1; i < k; ++i) {
            if (tmp[i] != tmp[i - 1]) {
                seq[seq_len++] = tmp[i];
            }
        }
        free(tmp);
    }

    /* n is 1‑based.  The pre‑computed sequence is guaranteed more than long enough. */
    return seq[n - 1];
}
