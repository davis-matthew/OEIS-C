/*
 * A189022  Apostol's third-order Möbius function mu_3(n).
 * https://oeis.org/A189022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   Apostol's third‑order Möbius function mu_3(n) Defined by:
 *            mu_3(n) = 0 if some prime p satisfies p^4 | n mu_3(n) = (-1)^r ,
 *            where r = #{p : exponent of p in n is exactly 3} with mu_3(1) =
 *            1. The function computes this value for any n >= 1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Use 64‑bit unsigned arithmetic for intermediate steps. */
    uint64_t x = (uint64_t)n;
    int r = 0;            /* parity of number of primes with exponent 3 */

    /* Factor 2 separately. */
    int e = 0;
    while ((x & 1ULL) == 0ULL) { /* x % 2 == 0 */
        x >>= 1;                 /* x /= 2 */
        e++;
    }
    if (e >= 4) return (seq_t)0;
    if (e == 3) r ^= 1;

    /* Trial division by odd numbers up to sqrt(x). */
    for (uint64_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) {
            e = 0;
            while (x % i == 0) {
                x /= i;
                e++;
            }
            if (e >= 4) return (seq_t)0;
            if (e == 3) r ^= 1;
        }
    }

    /* Remaining x > 1 is a prime factor with exponent 1. */
    /* No effect on r. */

    return r ? (seq_t)(-1) : (seq_t)1;
}
