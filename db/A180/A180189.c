/*
 * A180189  Number of permutations of [n] having exactly 1 circular
 *          succession. A circular succession in a permutation p of [n] is
 *          either a pair p(i), p(i+1), where p(i+1)=p(i)+1 or the pair p(n),
 *          p(1) if p(1)=p(n)+1.
 * https://oeis.org/A180189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A180189: a(n) = n*(n-1)*d(n-2) where d(k) is the derangement number of k */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(1) = 0, a(2) = 2*(2-1)*d(0) = 2, etc. */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* ensure we have derangements up to (n-2) */
    static seq_t *der = NULL;
    static idx_t der_cap = 0;   /* allocated capacity, number of slots */
    idx_t m = n - 2;             /* index for derangement number */

    if (der_cap <= m) {
        /* grow capacity to at least m+1, doubling strategy */
        idx_t new_cap = der_cap == 0 ? 2 : der_cap;
        while (new_cap <= m) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(der, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort (no graceful error recovery) */
            abort();
        }
        der = tmp;

        /* initialize new slots */
        for (idx_t i = der_cap; i < new_cap; ++i) {
            if (i == 0) {
                der[i] = (seq_t)1;
            } else if (i == 1) {
                der[i] = (seq_t)0;
            } else {
                seq_t sum = der[i - 1] + der[i - 2];
                der[i] = (seq_t)(i - 1) * sum;
            }
        }
        der_cap = new_cap;
    }

    /* compute answer */
    return (seq_t)n * (seq_t)(n - 1) * der[m];
}
