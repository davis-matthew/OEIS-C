/*
 * A014017  Inverse of 8th cyclotomic polynomial.
 * https://oeis.org/A014017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n/4) if n % 4 == 0, else 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A014017: a(4k) = (-1)^k, a(n) = 0 otherwise */
seq_t a(idx_t n)
{
    /* If n is not a multiple of 4, the result is 0 */
    if (n & 3)
        return (seq_t)0;

    /* n is a multiple of 4: compute k = n/4 */
    idx_t k = n >> 2;          /* division by 4 */

    /* (-1)^k: 1 if k even, -1 if k odd */
    return (k & 1) ? (seq_t)-1 : (seq_t)1;
}
