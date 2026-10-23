/*
 * A014901  a(1)=1, a(n) = 18*a(n-1) + n.
 * https://oeis.org/A014901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) + n; a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n = 0, return 0 for safety */

    seq_t res = 1;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        res = 18 * res + (seq_t)i;
    }
    return res;
}
