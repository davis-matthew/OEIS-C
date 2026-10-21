/*
 * A052955  a(2n) = 2*2^n - 1, a(2n+1) = 3*2^n - 1.
 * https://oeis.org/A052955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 2, a(n) = 2*a(n-2) + 1 (for n ≥ 2)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    seq_t a_prev2 = 1;   /* a(0) */
    seq_t a_prev1 = 2;   /* a(1) */
    seq_t a_curr = 0;
    for (idx_t i = 2; i <= n; ++i) {
        a_curr = (a_prev2 << 1) + 1;  /* 2 * a_prev2 + 1 */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
