/*
 * A004131  Modular postage stamp problem: largest m such that there exists an
 *          n-subset S of nonnegative integers such that 0,...,m-1 can be
 *          expressed as a mod-m sum of two distinct elements of S.
 * https://oeis.org/A004131
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6n - 24 for n >= 8; a(2)=1, a(3)=3, a(4)=6, a(5)=9,
 *            a(6)=13, a(7)=17
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 8) {
        static const seq_t small_values[] = {0, 0, 1, 3, 6, 9, 13, 17};
        return small_values[n];
    }
    return 6 * (seq_t)n - 24;
}
