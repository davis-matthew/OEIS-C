/*
 * A004305  Simple triangulations of a disk: column 4 of square array in
 *          A210664.
 * https://oeis.org/A004305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 15*n^5 + 103*n^4 + 405*n^3 + 976*n^2 + 1236*n +
 *            432)/144
 * Range:     u8 none, u16 n <= 3, u32 n <= 37, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;
    
    seq_t numerator = n6 + 15*n5 + 103*n4 + 405*n3 + 976*n2 + 1236*nn + 432;
    return numerator / 144;
}
