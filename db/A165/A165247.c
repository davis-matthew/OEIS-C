/*
 * A165247  a(n) = (10^n + 71)/9.
 * https://oeis.org/A165247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n + 71)/9 Recurrence: a(1) = 9, a(2) = 19, a(n) =
 *            11*a(n-1) - 10*a(n-2) for n >= 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)19;
    seq_t prev2 = (seq_t)9;   /* a(1) */
    seq_t prev1 = (seq_t)19;  /* a(2) */
    seq_t curr;
    for (idx_t i = 3; i <= n; ++i) {
        curr = 11 * prev1 - 10 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
