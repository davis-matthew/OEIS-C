/*
 * A180187  Number of successions in all the permutations p of [n] such that
 *          p(1)=1 and having no 3-sequences. A succession of a permutation p
 *          is a position i such that p(i +1) - p(i) = 1.
 * https://oeis.org/A180187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..⌊n/2⌋} k·binomial(n−k,k)·d(n−1−k), where d(j) is
 *            the j‑th derangement number
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* assumes seq_t and idx_t are defined elsewhere and that the utility
   function binomial(n,k) (./../utils/binomial.h) is available */
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* derangement values up to the current maximum needed */
    static seq_t *der = NULL;
    static size_t der_len = 0;   /* number of elements stored (indices 0 .. der_len-1) */

    /* initialise for the first call or when n == 0 (ignored by the OEIS offset) */
    if (der_len == 0) {
        der_len = 1;            /* we need at least d(0) */
        der = mem_new(seq_t, der_len);
        if (!der) return 0;     /* out of memory: silently return 0 */
        der[0] = 1;             /* d(0) = 1 */
    }

    /* extend der array if we need d values up to n-1 */
    if (der_len <= (size_t)(n - 1)) {
        size_t new_len = (size_t)n;   /* indices 0 .. n-1 inclusive */
        seq_t *tmp = mem_resize(seq_t, der, new_len);
        if (!tmp) return 0;           /* out of memory */
        der = tmp;

        /* compute missing derangements */
        for (size_t i = der_len; i < new_len; ++i) {
            if (i == 1) {
                der[i] = 0;           /* d(1) = 0 */
            } else {
                der[i] = (seq_t)(i - 1) * (der[i - 1] + der[i - 2]);
            }
        }
        der_len = new_len;
    }

    seq_t result = 0;
    idx_t kmax = n / 2;   /* floor(n/2) */

    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t binom = binomial(n - k, k);
        seq_t dval  = der[(size_t)(n - 1 - k)];
        seq_t term  = (seq_t)k * binom * dval;
        result += term;
    }

    return result;
}
