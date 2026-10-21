/*
 * A210729  a(n) = a(n-1) + a(n-2) + n + 3 with n>1, a(0)=1, a(1)=2.
 * https://oeis.org/A210729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(n) = a(n-1) + a(n-2) + n + 3 for n > 1 (OEIS
 *            A210729)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;

    seq_t prev2 = 1;  /* a(0) */
    seq_t prev1 = 2;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t curr = prev1 + prev2 + (seq_t)i + 3;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
