/*
 * A099990  a(n) = Moebius(2n+1).
 * https://oeis.org/A099990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Moebius(2n+1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute the Möbius function for an odd integer m >= 1.
   Uses trial division up to sqrt(m). */
static seq_t mobius_of(idx_t m)
{
    if (m == 1) return 1;                /* μ(1) = 1 */

    seq_t mu = 1;
    idx_t i = 2;

    for (; i <= m / i; ++i) {
        if (m % i == 0) {
            int exp = 0;
            while (m % i == 0) {
                m /= i;
                ++exp;
                if (exp > 1)            /* square factor found */
                    return 0;
            }
            mu = -mu;                   /* odd number of distinct primes */
        }
    }
    if (m > 1)                /* remaining prime factor */
        mu = -mu;

    return mu;
}

/* OEIS sequence A099990 */
seq_t a(idx_t n)
{
    /* 2n+1 is odd. Compute Möbius of that value. */
    idx_t m = n * (idx_t)2 + (idx_t)1;
    return mobius_of(m);
}
