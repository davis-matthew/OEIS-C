/*
 * A172212  Number of ways to place 3 nonattacking knights on a 3 X n board.
 * https://oeis.org/A172212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^3 - 45*n^2 + 122*n - 144)/2 for n>=4
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 823, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=1, a(2)=12, a(3)=36 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)36;

    seq_t sn = (seq_t)n;
    seq_t res = (seq_t)9 * sn * sn * sn
              - (seq_t)45 * sn * sn
              + (seq_t)122 * sn
              - (seq_t)144;
    return res / (seq_t)2;
}
