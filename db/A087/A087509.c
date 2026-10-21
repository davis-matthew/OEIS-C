/*
 * A087509  Number of k such that (k*n) == 2 (mod 3) for 0 <= k <= n.
 * https://oeis.org/A087509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n mod 3 == 0, otherwise (n + 1) / 3
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n % 3 == 0) {
        return (seq_t)0;
    } else {
        seq_t nn = (seq_t)n;
        return (nn + (seq_t)1) / (seq_t)3;
    }
}
