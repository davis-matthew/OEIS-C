/*
 * A002245  A (4,2)-sequence.
 * https://oeis.org/A002245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   A002245 is a (4,2)-sequence with 8 terms: 0,1,4,6,5,3,7,2
 *            (indices 0-7), finite sequence
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t terms[] = {0, 1, 4, 6, 5, 3, 7, 2};
    if (n < 8) {
        return terms[(long long)n];
    }
    return 0;
}
