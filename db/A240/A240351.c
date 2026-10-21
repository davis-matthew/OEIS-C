/*
 * A240351  Inverse of 64th cyclotomic polynomial.
 * https://oeis.org/A240351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if n mod 32 ≠ 0; otherwise a(n) = (-1)^(n/32)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is not a multiple of 32, the coefficient is 0. */
    if ((n & 31) != 0)
        return (seq_t)0;

    /* n is divisible by 32. Compute k = n/32. */
    idx_t k = n >> 5;  /* shift right by 5 bits is equivalent to division by 32. */

    /* Alternate sign every time k increases by 1. */
    return ((k & 1) == 0) ? (seq_t)1 : (seq_t)-1;
}
