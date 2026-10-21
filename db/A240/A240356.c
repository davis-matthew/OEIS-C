/*
 * A240356  Inverse of 73rd cyclotomic polynomial.
 * https://oeis.org/A240356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of the 73rd cyclotomic polynomial
   a(n) = 1  if n ≡ 0 (mod 73)
   a(n) = -1 if n ≡ 1 (mod 73)
   a(n) = 0  otherwise
   (The coefficients of the formal power series expansion of
    1/Φ_73(x) = (x−1)/(x^73−1).) */
seq_t a(idx_t n)
{
    /* Only the remainder modulo 73 matters. */
    idx_t r = n % (idx_t)73;
    if (r == (idx_t)0)
        return (seq_t)1;
    else if (r == (idx_t)1)
        return (seq_t)-1;
    else
        return (seq_t)0;
}
