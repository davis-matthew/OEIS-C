/*
 * A173438  Number of divisors d of number n such that d does not divide
 *          sigma(n).
 * https://oeis.org/A173438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) - tau(gcd(n, sigma(n)))
 * Range:     u8 n <= 2057, u16 n <= 9514, u32 n <= 9514, u64 n <= 9514,
 *            u128 n <= 9514, big any n
 * Helpers:   divisor_count_u64, seq_gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the helper headers that provide the required functions. */
#include <stdint.h>
#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n); */
#include "../utils/seq_gcd.h"    /* seq_t seq_gcd(seq_t a, seq_t b); */
#include "../utils/divisor_count_u64.h" /* seq_t divisor_count_u64(uint64_t n); */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For completeness handle n == 0. */
    if (n == 0)
        return 0;

    /* Sum of divisors of n. */
    seq_t sigma = sigma_of(n);

    /* Greatest common divisor of n and sigma(n). */
    seq_t g = seq_gcd((seq_t)n, sigma);

    /* Number of divisors of n and of gcd(n, sigma(n)). */
    seq_t tau_n = divisor_count_u64(n);
    seq_t tau_g = divisor_count_u64((uint64_t)g);  /* g ≤ n, so fits in uint64_t */

    /* Return the difference. */
    return tau_n - tau_g;
}
