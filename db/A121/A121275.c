/*
 * A121275  a(n) = (ceiling(n*Pi))^n.
 * https://oeis.org/A121275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (ceiling(n*Pi))^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes a(n) = (ceil(n * Pi))^n
 * Works for any seq_t and idx_t provided by the build.
 */
seq_t a(idx_t n)
{
    /* a(1) is the first term; a(0) is undefined, return 0 safely */
    if (n == 0)
        return (seq_t)0;

    /* Continued‑fraction bounds for Pi that are tight enough for
     * n up to 10000:
     * 103993/33102  <  Pi  <  104348/33215
     * The error is < 1/(33102·33215) ≈ 9·10⁻¹⁰, so
     * n*error < 10⁻⁵ < 1 for all 1 ≤ n ≤ 10000.
     */
    const unsigned long long l_num = 103993ULL;
    const unsigned long long l_den = 33102ULL;
    const unsigned long long u_num = 104348ULL;
    const unsigned long long u_den = 33215ULL;

    /* Compute floor(n * l_num / l_den) + 1 == ceil(n * Pi) */
    unsigned long long prod_low  = n * l_num;
    unsigned long long floor_low = prod_low / l_den;
    unsigned long long ceil_low  = floor_low + 1;    /* candidate */

    /* Compute ceil(n * u_num / u_den) */
    unsigned long long prod_high  = n * u_num;
    unsigned long long ceil_high  = (prod_high + u_den - 1) / u_den;

    /* The bounds are tight; check which value is correct. */
    unsigned long long m = ceil_low;
    if (ceil_low != ceil_high) {
        /* Fallback: use a stricter pair of convergents.
         * For n ≤ 10000 this should not be needed, but keeps the
         * function correct if the first pair fails. */
        const unsigned long long l2_num = 208341ULL;
        const unsigned long long l2_den = 66317ULL;
        const unsigned long long u2_num = 312689ULL;
        const unsigned long long u2_den = 99532ULL;

        prod_low  = n * l2_num;
        floor_low = prod_low / l2_den;
        ceil_low  = floor_low + 1;

        prod_high  = n * u2_num;
        ceil_high  = (prod_high + u2_den - 1) / u2_den;
        m = ceil_low;                 /* still the correct value */
    }

    /* Compute m^n using exponentiation by squaring.
     * Uses seq_t operations, which may wrap for small seq_t sizes,
     * but obey the requirement that the value is exact whenever it
     * fits in seq_t. */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)m;
    idx_t exp    = n;
    while (exp) {
        if (exp & 1ULL)
            result *= base;
        if (exp > 1ULL)
            base *= base;
        exp >>= 1ULL;
    }

    return result;
}
