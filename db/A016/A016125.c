/*
 * A016125  Expansion of 1/((1-x)*(1-12*x)).
 * https://oeis.org/A016125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 12*a(n-1) + 1 , a(0) = 1 (equivalently (12^(n+1)-1)/11)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    while (n--)
        res = res * 12 + 1;
    return res;
}
