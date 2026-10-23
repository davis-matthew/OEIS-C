/*
 * A034723  a(n) is the n-th sextic factorial number divided by 3.
 * https://oeis.org/A034723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (∏_{k=1..n} (6k-3)) / 3 (OEIS A034723)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i) {
        prod = prod * (seq_t)(6 * i - 3);
    }
    return prod / 3;
}
