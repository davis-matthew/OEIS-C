/*
 * A188154  Number of 9-step self-avoiding walks on an n X n square summed
 *          over all starting positions.
 * https://oeis.org/A188154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5916*n^2 - 38192*n + 55600 for n > 7 Initial values:
 *            a(1)=0, a(2)=0, a(3)=40, a(4)=2640, a(5)=14520, a(6)=39792,
 *            a(7)=78168
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 7)
    {
        static const seq_t table[7] = {0, 0, 40, 2640, 14520, 39792, 78168};
        return table[n - 1];
    }
    seq_t nt = (seq_t)n;
    return 5916 * nt * nt - 38192 * nt + 55600;
}
