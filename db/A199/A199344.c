/*
 * A199344  Least integer > n having a digital sum larger than that of n.
 * https://oeis.org/A199344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n+1 unless n=9 (mod 10); otherwise
 *            a(n)=n+10^valuation(n+1,10)
 * Range:     u8 n <= 198, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n + 1;
    if ((n1 % 10) != 0)          /* simple case */
        return (seq_t)n + 1;

    seq_t pow10 = 1;              /* 10^k, initially 1 (=10^0) */
    /* grow pow10 while it still divides n1 */
    while (pow10 <= n1 / 10 && (n1 % (pow10 * 10)) == 0)
        pow10 *= 10;

    return (seq_t)n + pow10;      /* add the exact 10^k */
}
