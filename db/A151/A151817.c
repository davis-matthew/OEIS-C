/*
 * A151817  a(n) = 2*(2*n)!/n!.
 * https://oeis.org/A151817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(2*n)!/n! (Recurrence: a(0)=2, a(n)=2*(2*n-1)*a(n-1)
 *            for n≥1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = (seq_t)2;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t factor = (seq_t)2 * (((seq_t)2 * i) - (seq_t)1);
        ans = ans * factor;
    }
    return ans;
}
