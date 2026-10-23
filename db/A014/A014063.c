/*
 * A014063  Inverse of 54th cyclotomic polynomial.
 * https://oeis.org/A014063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n ≡ 0 or 9 (mod 27) and ⌊n/27⌋ is even, a(n) = –1 if
 *            n ≡ 0 or 9 (mod 27) and ⌊n/27⌋ is odd, a(n) = 0 otherwise. This
 *            produces the period 54 of OEIS A014063
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 54, the period. */
    idx_t m = n % 54;
    /* Positive terms at positions 0 and 9 mod 54. */
    if (m == 0 || m == 9) {
        return (seq_t)1;
    }
    /* Negative terms at positions 27 and 36 mod 54. */
    if (m == 27 || m == 36) {
        return (seq_t)(-1);
    }
    /* All other positions are zero. */
    return (seq_t)0;
}
