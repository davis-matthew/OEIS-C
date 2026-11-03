/*
 * A069901  Smallest prime factor of n-th triangular number.
 * https://oeis.org/A069901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime factor of n*(n+1)/2, with a(1)=1
 * Range:     u8 n <= 24, u16 n <= 537, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 8960 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t smallest_prime_factor_of_tri(idx_t n)
{
    /* Compute the nth triangular number, using seq_t to avoid overflow of
     * intermediate products.  The definition is
     *
     *   T_n = n*(n + 1)/2
     *
     * For n = 1 we return 1, because the triangular number is 1 which has
     * no prime divisor > 1.  For all other n we find the smallest prime
     * factor by trial division up to sqrt(T_n).  Since n ≤ 10 000 the
     * triangular number is ≤ 50 005 000, so the loop terminates very
     * quickly.
     */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    if (tri <= 1)
        return tri;              /* 1 → a(1) = 1 */

    /* Check divisibility by 2 first. */
    const seq_t two = 2;
    if ((tri % two) == 0)
        return two;

    /* Trial division by odd numbers starting at 3.  Since the
     * triangular number is small, d*d overflows seq_t are not an issue. */
    for (seq_t d = 3; d * d <= tri; d += 2) {
        if ((tri % d) == 0)
            return d;          /* first divisor found is the smallest prime factor */
    }

    /* If no divisor was found, the triangular number itself is prime. */
    return tri;
}

seq_t a(idx_t n)
{
    return smallest_prime_factor_of_tri(n);
}
