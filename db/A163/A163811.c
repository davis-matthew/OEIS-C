/*
 * A163811  Expansion of (1 - x) * (1 - x^10) / ((1 - x^5) * (1 - x^6)) in
 *          powers of x.
 * https://oeis.org/A163811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n = 0, -1 if n mod 6 = 1, 1 if n mod 6 = 5, 0
 *            otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    idx_t r = n % (idx_t)6;
    if (r == 0) return (seq_t)0;   /* multiples of 6 except 0 */
    if (r == 1) return (seq_t)-1;  /* positions 1,7,13,… */
    if (r == 5) return (seq_t)1;   /* positions 5,11,17,… */
    return (seq_t)0;               /* other residues */
}
