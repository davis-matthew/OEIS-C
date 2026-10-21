/*
 * A229356  Total sum of n-th powers of parts in all partitions of 5.
 * https://oeis.org/A229356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12 + 4*2^n + 2*3^n + 4^n + 5^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;
    seq_t pow3 = 1;
    seq_t pow4 = 1;
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;
        pow3 *= 3;
        pow4 *= 4;
        pow5 *= 5;
    }
    seq_t res = 12;
    res += 4 * pow2;
    res += 2 * pow3;
    res += pow4;
    res += pow5;
    return res;
}
