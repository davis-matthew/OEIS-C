/*
 * A077415  a(n) = n*(n+2)*(n-2)/3.
 * https://oeis.org/A077415
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+2)*(n-2)/3
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 2187, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* offset is 2; values below not used */
    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn + 2) * (nn - 2) / 3;
    return val;
}
