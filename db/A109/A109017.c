/*
 * A109017  a(n) = Kronecker symbol (-6/n).
 * https://oeis.org/A109017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Kronecker symbol (-6/n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) = (−6 / n) as defined in the OEIS entry for A109017.
 * The function works for any non‑negative n.  It uses only integer
 * arithmetic and short computations. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* (−6/0) is defined as 0 */

    seq_t res = 1;                 /* accumulator for the sign */

    /* (−1 / n)  – use Legendre/Jacobi rule for odd part of n */
    idx_t m = n;
    while ((m & 1U) == 0) m >>= 1; /* strip powers of 2, n > 0 guarantees m ≠ 0 */
    /* (−1 / m) = (−1)^((m−1)/2)  */
    int exp1 = ((m - 1U) >> 1U) & 1U;      /* parity of (m−1)/2 */
    seq_t s1 = exp1 ? -1 : 1;
    res *= s1;

    /* ( 2 / n)  – zero if n is even, otherwise Jacobi formula */
    if ((n & 1U) == 0) return 0U;          /* factor is 0, whole product 0 */
    /* for odd n, (2/n) = (−1)^((n^2−1)/8) */
    int mod8 = n & 7U;                     /* n % 8 */
    seq_t s2 = (mod8 == 1 || mod8 == 7) ? 1 : -1;
    res *= s2;

    /* ( 3 / n)  – zero if 3 | n, otherwise Legendre/Jacobi rule */
    if ((n % 3U) == 0) return 0U;          /* factor is 0, whole product 0 */
    int mod12 = n % 12U;                   /* n % 12 (n not divisible by 3) */
    /* (3/n) = 1 if n % 12 == 1 or 11, else -1 */
    seq_t s3 = (mod12 == 1 || mod12 == 11) ? 1 : -1;
    res *= s3;

    return res;
}
