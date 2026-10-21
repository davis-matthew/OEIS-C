/*
 * A212740  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          max{w,x,y,z}<2*min{w,x,y,z}.
 * https://oeis.org/A212740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 / 8 if n is even a(n) = (n^4 + 6 n^2 + 1) / 8 if n is
 *            odd (Derived from the explicit formula in the OEIS entry.)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;          // n^2
    seq_t n4 = n2 * n2;                // n^4
    if ((n & 1) == 0) {
        return n4 / (seq_t)8;
    } else {
        return (n4 + (seq_t)6 * n2 + (seq_t)1) / (seq_t)8;
    }
}
