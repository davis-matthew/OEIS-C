/*
 * A146966  a(n) = ((6 + sqrt(7))^n + (6 - sqrt(7))^n) / 2.
 * https://oeis.org/A146966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 29*a(n-2), a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)6;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)6;   /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2u; i <= n; ++i) {
        cur = prev1 * (seq_t)12 - prev2 * (seq_t)29;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
