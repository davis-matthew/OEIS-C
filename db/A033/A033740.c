/*
 * A033740  Expansion of Product_{d | 28} theta_3(q^d).
 * https://oeis.org/A033740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 726, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A033740: expansion of ∏_{d|28} θ₃(q^d)
   where θ₃(q) = ∑_{k∈ℤ} q^{k²}.
   The coefficient a(n) equals the number of integer 6‑tuples
   (k₁,k₂,k₄,k₇,k₁₄,k₂₈) such that 
       1·k₁² + 2·k₂² + 4·k₄² + 7·k₇² + 14·k₁₄² + 28·k₂₈² = n
   each non‑zero coordinate contributing a factor 2 (±k).  The
   coefficients are obtained by convolving the sparse sequences
   c_d[n] = 1 if n = 0, 2 if n = d·s² (s>0), else 0,
   over all divisors d = {1,2,4,7,14,28}.  This implementation
   uses an incremental memoised convolution and is O(n·√n)
   for the largest n requested.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of the already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_limit = 0;   /* number of entries in cache */

    /* if we already have the answer, return it */
    if (n < cache_limit)
        return cache[n];

    /* ----------------------------------------------------------
       compute a[0..n] from scratch (only when a larger n is asked)
       ---------------------------------------------------------- */
    const idx_t divisors[] = {1, 2, 4, 7, 14, 28};
    const size_t nd = sizeof(divisors) / sizeof(divisors[0]);

    /* allocate temporary arrays */
    seq_t *seq = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *tmp = (seq_t*)malloc((n + 1) * sizeof(seq_t));

    /* initialise with the identity element of convolution */
    for (idx_t i = 0; i <= n; ++i) seq[i] = 0;
    seq[0] = 1;

    /* convolve successively with each c_d */
    for (size_t di = 0; di < nd; ++di) {
        idx_t d = divisors[di];

        /* zero temporary array */
        for (idx_t i = 0; i <= n; ++i) tmp[i] = 0;

        /* m = 0 contribution: value 1 */
        for (idx_t i = 0; i <= n; ++i)
            tmp[i] += seq[i];

        /* m = d·s², s ≥ 1 : value 2 */
        for (idx_t s = 1; ; ++s) {
            idx_t m = d * s * s;
            if (m > n) break;
            seq_t val = (seq_t)2;
            for (idx_t i = m; i <= n; ++i) {
                tmp[i] += seq[i - m] * val;
            }
        }

        /* copy tmp back into seq for the next divisor */
        for (idx_t i = 0; i <= n; ++i) seq[i] = tmp[i];
    }

    /* free the old cache (if any) and replace it */
    free(cache);
    free(tmp);
    cache = seq;
    cache_limit = n + 1;

    return cache[n];
}
