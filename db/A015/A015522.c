/*
 * A015522  Inverse of 1513th cyclotomic polynomial.
 * https://oeis.org/A015522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n mod 1513 ∈ [0,16], a(n) = -1 if n mod 1513 ∈
 *            [89,105], otherwise a(n) = 0
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from 1/Φ_1513(x) = (1-x)^(-1)(1-x^17)(1-x^89)(1-x^1513)^(-1) */

seq_t a(idx_t n) {
    const idx_t PERIOD = 1513u;
    idx_t r = n % PERIOD;           /* remainder modulo 1513 */
    if (r <= 16u)         return (seq_t)1;
    if (r >= 89u && r <= 105u) return (seq_t)-1;
    return (seq_t)0;
}
