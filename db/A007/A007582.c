/*
 * A007582  a(n) = 2^(n-1)*(1+2^n).
 * https://oeis.org/A007582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^(n-1)*(1+2^n), i.e. with p = 2^(n-1): a(n) = p*(1+2*p)
 *            for n >= 1, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p, r;
    idx_t i;
    if (n == 0)
        return (seq_t)1;
    p = (seq_t)1;
    for (i = 1; i < n; i++)
        p = p * 2;
    r = p * 2;
    r = r + 1;
    r = p * r;
    return r;
}
