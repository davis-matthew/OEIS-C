/*
 * A188946  Binomial partial sums of binomial(2n,n)*binomial(3n,n) (A006480).
 * https://oeis.org/A188946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(2k,k) *
 *            binomial(3k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses iterative multiplicative updates for the three
   binomial coefficients to achieve O(n) time per call. */

#include <stdint.h>
#include <stdbool.h>

/* The build supplies the following integer types:
   - idx_t : unsigned integer type for indices (uint64_t).
   - seq_t : integer type for sequence values (uint8_t ... uint64_t,
             unsigned __int128, or arbitrary-precision class).
   The following helper functions are provided elsewhere if needed:
   - No additional helpers are required for this implementation. */

seq_t a(idx_t n)
{
    /* Local accumulators */
    seq_t sum          = (seq_t)0;   /* running total */
    seq_t bin_nk       = (seq_t)1;   /* C(n,0) */
    seq_t bin_2k_k     = (seq_t)1;   /* C(0,0)   */
    seq_t bin_3k_k     = (seq_t)1;   /* C(0,0)   */

    for (idx_t k = 0; k <= n; ++k) {
        /* Update binomial coefficients for k > 0 */
        if (k > 0) {
            /* C(n,k) = C(n,k-1) * (n-k+1) / k */
            bin_nk = bin_nk *
                     ((seq_t)(n - k + 1)) /
                     ((seq_t)k);

            /* C(2k,k) = C(2(k-1),(k-1)) * (2k)(2k-1) / (k*k) */
            bin_2k_k = bin_2k_k *
                       ((seq_t)(2 * k)) *
                       ((seq_t)(2 * k - 1)) /
                       (((seq_t)k) * ((seq_t)k));

            /* C(3k,k) = C(3(k-1),(k-1)) *
               (3k)(3k-1)(3k-2) / (2*k*k*(2k-1)) */
            seq_t num = ((seq_t)(3 * k)) *
                        ((seq_t)(3 * k - 1)) *
                        ((seq_t)(3 * k - 2));
            seq_t den = ((seq_t)2) *
                        ((seq_t)k) *
                        ((seq_t)k) *
                        ((seq_t)(2 * k - 1));
            bin_3k_k = bin_3k_k * num / den;
        }

        /* Add current term to sum */
        seq_t term = bin_nk * bin_2k_k;
        term = term * bin_3k_k;
        sum = sum + term;
    }

    return sum;
}
