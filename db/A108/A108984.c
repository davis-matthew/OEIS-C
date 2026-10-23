/*
 * A108984  Inverse binomial transform of A003951.
 * https://oeis.org/A108984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 7*a(n-2), a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 8;
    seq_t prev2 = 1;
    seq_t prev1 = 8;
    idx_t i;
    for (i = 2; i <= n; ++i) {
        seq_t cur = 6 * prev1 + 7 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
