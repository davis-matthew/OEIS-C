/*
 * A100530  Numbers == 0,2,5,9 modulo 10.
 * https://oeis.org/A100530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Mod[n,4] + 1; a(0)=0
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
        sum += (seq_t)((i % 4ull) + 1ull);
    return sum;
}
