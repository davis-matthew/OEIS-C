/*
 * A058396  Expansion of ((1-x)/(1-2*x))^3.
 * https://oeis.org/A058396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*(n+7)*2^(n-4) for n>0, a(0)=1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t val = (seq_t)(n + 2) * (n + 7);

    if (n < 4) /* divide by 2^(4-n) */
        return val >> (4 - n);

    return val << (n - 4);
}
