/*
 * A119969  a(n) = Sum_{k>=0} C(2^k-1,n-2*(2^k-1)).
 * https://oeis.org/A119969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} C(k, n-2k) * (Catalan(k) mod 2)
 * Range:     u8 n <= 32, u16 n <= 65, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here (Catalan(k) mod 2) = 1 iff k+1 is a power of two. */
#include "../utils/binom.h"

seq_t a(idx_t n) {
    seq_t sum = 0;                      /* accumulator for the sum   */
    idx_t max_k = n / 2;                /* floor(n/2)                 */

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t r = n - 2 * k;            /* n - 2k  (0 ≤ r ≤ k here).  */
        if (r > k) continue;           /* skip if the binomial is zero */

        seq_t term = binom(k, r);      /* exact binomial coefficient */

        /* Catalan(k) is odd iff k+1 is a power of two */
        if (((k + 1) & k) == 0)
            sum += term;
    }
    return sum;
}
