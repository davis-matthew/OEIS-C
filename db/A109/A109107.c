/*
 * A109107  a(n) = (1/sqrt(26))((5+sqrt(26))^(n+1)-(5-sqrt(26))^(n+1)).
 * https://oeis.org/A109107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1)+a(n-2), a(0)=2, a(1)=20
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 2;
    if (n == 1) return 20;
    seq_t prev2 = 2; /* a(0) */
    seq_t prev1 = 20; /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * 10 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
