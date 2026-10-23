/*
 * A132417  a(16j+i) = 8(16j+i) + e_i, for j >= 0, 0 <= i <= 15, where e_0,
 *          ..., e_15 are 2, -2, -6, -10, -14, -18, -22, -26, -30, -34, -38,
 *          -42, -46, -50, -54, 6.
 * https://oeis.org/A132417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n + e[n%16] where e[0]=2, e[1]=-2, e[2]=-6, e[3]=-10, *
 *            e[4]=-14, e[5]=-18, e[6]=-22, e[7]=-26, e[8]=-30, e[9]=-34, *
 *            e[10]=-38, e[11]=-42, e[12]=-46, e[13]=-50, e[14]=-54, e[15]=6
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const int e[16] = {2, -2, -6, -10, -14, -18, -22, -26, -30, -34,
                              -38, -42, -46, -50, -54, 6};
    int ei = e[n % 16];
    seq_t res = ((seq_t)n << 3);        /* 8 * n */
    if (ei >= 0)
        res += (seq_t)ei;
    else
        res -= (seq_t)(-ei);
    return res;
}
