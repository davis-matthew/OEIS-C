/*
 * A008488  Expansion of (1-x^6) / (1-x)^6.
 * https://oeis.org/A008488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 15*n^2 + 8)/4 for n > 0; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 286, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sn  = (seq_t)n;
    seq_t sq2 = sn * sn;
    seq_t sq4 = sq2 * sq2;
    seq_t num = sq4 + (seq_t)15 * sq2 + (seq_t)8;
    return num / (seq_t)4;
}
