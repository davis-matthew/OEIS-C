/*
 * A125287  a(n) = mu(n) * A000217(n).
 * https://oeis.org/A125287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = mu(n) * n*(n+1)/2 (triangular number times Möbius
 *            function)
 * Range:     i8 n <= 16, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Compute Möbius function µ(n) for n >= 1 */
static int mobius_uint64(uint64_t n)
{
    if (n == 1) return 1;
    int prime_factors = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
                if (exp > 1) {
                    return 0;          /* square factor found */
                }
            }
            ++prime_factors;
        }
    }
    if (m > 1) {
        ++prime_factors;              /* remaining prime */
    }
    return (prime_factors % 2 == 0) ? 1 : -1;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    /* triangular number T(n) = n*(n+1)/2 as seq_t */
    seq_t tri = ((seq_t)n) * ((seq_t)n + 1) / 2;

    /* Möbius function μ(n) */
    int mu_int = mobius_uint64(n);
    seq_t mu = (seq_t)mu_int;

    return mu * tri;
}
