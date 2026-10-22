/*
 * A004395  Ratios of successive terms are 1,1,2,3,3,4,5,5,6,7,7,...
 * https://oeis.org/A004395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of ratio sequence 1,1,2,3,3,4,5,5,6,7,7,
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 0; i < n; i++) {
        idx_t m = (i + 3) / 3;
        seq_t ratio;
        if (i < 3*m - 1) {
            ratio = (seq_t)(2*m - 1);
        } else {
            ratio = (seq_t)(2*m);
        }
        result *= ratio;
    }
    return result;
}
