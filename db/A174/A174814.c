/*
 * A174814  a(n) = n*(n+1)*(5*n+1)/3.
 * https://oeis.org/A174814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(5*n+1)/3
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 1370, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t a1 = n0;
    seq_t b1 = n0 + 1;
    seq_t c1 = 5 * n0 + 1;
    if (a1 % 3 == 0)
        a1 /= 3;
    else if (b1 % 3 == 0)
        b1 /= 3;
    else
        c1 /= 3;
    return a1 * b1 * c1;
}
