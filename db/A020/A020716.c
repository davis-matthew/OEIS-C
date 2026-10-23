/*
 * A020716  Pisot sequences E(6,8), P(6,8).
 * https://oeis.org/A020716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) + 1, a(0)=6, a(1)=8, a(2)=11, a(3)=15
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 72, u64 n <= 150,
 *            u128 n <= 308, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)15;

    seq_t cyc[4];
    cyc[0] = (seq_t)6;
    cyc[1] = (seq_t)8;
    cyc[2] = (seq_t)11;
    cyc[3] = (seq_t)15;

    for (idx_t i = 4; i <= n; ++i)
    {
        cyc[i % 4] = cyc[(i - 2) % 4] + cyc[(i - 3) % 4] + (seq_t)1;
    }

    return cyc[n % 4];
}
