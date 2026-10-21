/*
 * A076334  Differences between successive squarefree kernels.
 * https://oeis.org/A076334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = rad(n+1) - rad(n) where rad(m) = product of distinct
 *            prime factors of m (square‑free kernel)
 * Range:     i8 n <= 142, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the squarefree kernel (radical) of an integer m */
static seq_t rad_of(idx_t m)
{
    seq_t rad = 1;
    idx_t tmp = m;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            rad *= (seq_t)p;                /* include prime factor once */
            while (tmp % p == 0)            /* remove all powers of p */
                tmp /= p;
        }
    }
    if (tmp > 1)                           /* remaining prime > sqrt(m) */
        rad *= (seq_t)tmp;
    return rad;
}

seq_t a(idx_t n)
{
    /* n is 1‑based index; a(1) = rad(2) - rad(1) */
    seq_t rad_n   = rad_of(n);
    seq_t rad_np1 = rad_of(n + 1);
    return rad_np1 - rad_n;
}
