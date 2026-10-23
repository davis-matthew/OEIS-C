/*
 * A022527  Nexus numbers: a(n) = (n+1)^11 - n^11.
 * https://oeis.org/A022527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^11 - n^11
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 6, u64 n <= 65, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t coef[11] = {1, 11, 55, 165, 330, 462, 462, 330, 165, 55, 11};
    seq_t res = coef[10];
    for (int k = 9; k >= 0; --k)
        res = res * (seq_t)n + coef[k];
    return res;
}
