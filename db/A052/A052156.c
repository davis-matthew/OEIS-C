/*
 * A052156  Number of compositions of n into 2*j-1 kinds of j's for all j>=1.
 * https://oeis.org/A052156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 for n = 0,1 a(n) = 4 * 3^(n-2) for n >= 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    seq_t pow3 = (seq_t)1;
    idx_t exp = n - 2;
    for (idx_t i = 0; i < exp; ++i)
        pow3 *= (seq_t)3;

    return pow3 * (seq_t)4;
}
