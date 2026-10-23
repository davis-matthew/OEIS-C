/*
 * A008877  3x+1 sequence starting at 57.
 * https://oeis.org/A008877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=57; for n>0: a(n) = 3*a(n-1)+1 if a(n-1) is odd, else
 *            a(n-1)/2
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 57;
    if (n == 0) return val;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (val & 1)
            val = val * 3 + 1;
        else
            val = val / 2;
    }
    return val;
}
