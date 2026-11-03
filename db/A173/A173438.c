/*
 * A173438  Number of divisors d of number n such that d does not divide
 *          sigma(n).
 * https://oeis.org/A173438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) - tau(gcd(n, sigma(n)))
 * Range:     u8 n <= 2057, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 114 terms through n = 9514 (2026-11-01)
 * Helpers:   gcd, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the helper headers that provide the required functions. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/num_divisors.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For completeness handle n == 0. */
    if (n == 0)
        return 0;

    /* Sum of divisors of n. */
    seq_t sigma = sigma_of(n);

    /* Greatest common divisor of n and sigma(n). */
    seq_t g = gcd((seq_t)n, sigma);

    /* Number of divisors of n and of gcd(n, sigma(n)). */
    seq_t tau_n = num_divisors(n);
    seq_t tau_g = num_divisors((uint64_t)g);  /* g ≤ n, so fits in uint64_t */

    /* Return the difference. */
    return tau_n - tau_g;
}
