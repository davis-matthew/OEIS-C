/*
 * A055850  a(n) = 3*a(n-1) - a(n-2) with a(0)=1, a(1)=10.
 * https://oeis.org/A055850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2), a(0) = 1, a(1) = 10
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    seq_t prev2 = (seq_t)1; /* a(n-2) */
    seq_t prev1 = (seq_t)10;/* a(n-1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)3 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
