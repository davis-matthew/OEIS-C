/*
 * A082010  a(n) = n/2 if n is even, otherwise floor(8*n/5)+1.
 * https://oeis.org/A082010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/2 if n is even, otherwise floor(8*n/5)+1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t res;
    if ((n & 1) == 0)
        res = m / 2;
    else
        res = m * 8 / 5 + 1;
    return res;
}
