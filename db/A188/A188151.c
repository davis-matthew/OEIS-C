/*
 * A188151  Number of 6-step self-avoiding walks on an n X n square summed
 *          over all starting positions.
 * https://oeis.org/A188151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=0, a(3)=128, a(4)=800, a(n)=284*n*n - 1228*n + 1152
 *            for n>4
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1 || n == 2) return 0;
    if (n == 3)           return 128;
    if (n == 4)           return 800;

    seq_t nn = (seq_t)n;
    seq_t res = (seq_t)284 * nn * nn;
    res -= (seq_t)1228 * nn;
    res += (seq_t)1152;
    return res;
}
