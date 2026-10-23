/*
 * A015406  Inverse of 1397th cyclotomic polynomial.
 * https://oeis.org/A015406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for n mod 1397 in [0,10], a(n) = -1 for n mod 1397 in
 *            [127,137], otherwise a(n) = 0
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t p = 1397u;
    idx_t r = n % p;
    if (r <= 10u) return (seq_t)1;
    if (r >= 127u && r <= 137u) return (seq_t)-1;
    return (seq_t)0;
}
