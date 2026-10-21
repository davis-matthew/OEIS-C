/*
 * A063321  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          48 ).
 * https://oeis.org/A063321
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ((-(-1)^n) + 54*n - 117) / 2 for n >= 3 Special case:
 *            a(2) = -1 (Derived from the conjectured closed form and matches
 *            the provided terms.)
 * Range:     i8 n <= 4, i16 n <= 831, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 2) {
        return (seq_t)-1;
    }
    /* (-(-1)^n) is -1 for even n, +1 for odd n */
    seq_t part = (n % 2 == 0) ? (seq_t)-1 : (seq_t)1;
    part += (seq_t)54 * (seq_t)n;
    part -= 117;
    return part / 2;
}
