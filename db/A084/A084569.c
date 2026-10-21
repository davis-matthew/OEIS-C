/*
 * A084569  Partial sums of A084570.
 * https://oeis.org/A084569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (-1)^n/8 + (n^4 + 6*n^3 + 17*n^2 + 30*n + 21)/24
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;           // n^2
    seq_t n3 = n2 * tn;           // n^3
    seq_t n4 = n3 * tn;           // n^4

    seq_t s = n4 + (seq_t)6 * n3 + (seq_t)17 * n2 + (seq_t)30 * tn + (seq_t)21;
    seq_t num;
    if ((n & 1) == 0) {           // n even → add 3
        num = s + (seq_t)3;
    } else {                      // n odd → subtract 3
        num = s - (seq_t)3;
    }
    return num / ((seq_t)24);
}
