/*
 * A240357  Inverse of 74th cyclotomic polynomial.
 * https://oeis.org/A240357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 37 = 0 or 1, with sign (−1)^(⌊n/37⌋),
 *            otherwise a(n) = 0. Period = 74
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor division and remainder by 37. */
    idx_t m = n / 37;
    idx_t r = n % 37;

    /* Non‑zero only for r = 0 or 1. */
    if (r < 2) {
        /* Sign flips every 37 positions. */
        seq_t sign = (m & 1) ? -((seq_t)1) : (seq_t)1;
        return sign;
    }
    return 0;
}
