/*
 * A203601  a(0)=1, a(n+1) = (a(n)*7) XOR a(n).
 * https://oeis.org/A203601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1, a(n+1) = (a(n)*7) XOR a(n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        val = ((val * (seq_t)7) ^ val);
    }
    return val;
}
