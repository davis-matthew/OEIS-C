/*
 * A014880  Inverse of 871st cyclotomic polynomial.
 * https://oeis.org/A014880
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

/* 
 *  Inverse of 871st cyclotomic polynomial (periodic with period 871).
 *  a(n) =  1  for n mod 871 in [0,12]
 *  a(n) = -1  for n mod 871 in [67,79]
 *  a(n) =  0  otherwise
 */
seq_t a(idx_t n)
{
    idx_t m = n % 871;                /* period 871 */
    if (m <= 12u) {
        return (seq_t)1;
    }
    if (m >= 67u && m <= 79u) {
        return (seq_t)(-1);
    }
    return (seq_t)0;
}
