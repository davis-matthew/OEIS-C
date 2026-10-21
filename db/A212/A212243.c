/*
 * A212243  Number of (w,x,y,z) with all terms in {1,...,n} and 2wx+yz=n.
 * https://oeis.org/A212243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{w=1}^{n} sum_{x=1}^{⌊(n-1)/(2w)⌋} τ(n - 2*w*x) where
 *            τ(m) is the number of positive divisors of m
 * Range:     u8 n <= 35, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* No solutions for n = 0, 1, or 2 (handled automatically by the loops). */
    if (n == 0)
        return 0;

    /* Lazily compute divisor counts τ(m) up to the current n. */
    static unsigned int *tau = NULL;
    static idx_t tau_size = 0;      /* currently allocated size (entries 0..tau_size-1) */
    if (n + 1 > tau_size) {
        /* Allocate a larger array and recompute τ values from scratch.  */
        unsigned int *new_tau = (unsigned int *)malloc((size_t)(n + 1) * sizeof(unsigned int));
        if (!new_tau) return 0;  /* In practice, this should not happen. */
        /* Initialize to zero. */
        for (idx_t i = 0; i <= n; ++i) new_tau[i] = 0;
        /* Sieve to compute τ(m) for 1 ≤ m ≤ n. */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i)
                ++new_tau[j];
        }
        /* Replace old table. */
        free(tau);
        tau = new_tau;
        tau_size = n + 1;
    }

    /* Sum over all admissible (w, x) pairs. */
    seq_t sum = 0;
    for (idx_t w = 1; w <= n; ++w) {
        /* 2*w*x must not exceed n-1.  Compute maximal x.   */
        idx_t max_x = (n - 1) / (2 * w);
        for (idx_t x = 1; x <= max_x; ++x) {
            idx_t m = n - 2 * w * x;      /* 1 ≤ m ≤ n-2 */
            sum += (seq_t)tau[m];
        }
    }
    return sum;
}
