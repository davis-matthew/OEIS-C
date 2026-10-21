/*
 * A218743  a(n) = (40^n - 1)/39.
 * https://oeis.org/A218743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (40^n - 1) / 39 Recurrence: a(0) = 0, a(n) = 40 * a(n-1)
 *            + 1 for n >= 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        val = val * (seq_t)40 + 1;
    }
    return val;
}
