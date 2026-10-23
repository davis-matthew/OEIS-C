/*
 * A174239  a(n) = (3*n + 1 + (-1)^n*(n+3))/4.
 * https://oeis.org/A174239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + 1 + (-1)^n*(n+3))/4 Equivalent simple form: a(n) =
 *            n + 1 if n is even a(n) = (n - 1) / 2 if n is odd
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n & (idx_t)1)) {                       /* odd */
        return (((seq_t)n) - 1) / 2;
    } else {                                   /* even */
        return ((seq_t)n) + 1;
    }
}
