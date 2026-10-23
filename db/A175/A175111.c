/*
 * A175111  a(n) = ((2*n+1)^5+(-1)^n)/2.
 * https://oeis.org/A175111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((2*n+1)^5 + (-1)^n)/2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (2*n + 1) */
    seq_t base = ((seq_t)2 * (seq_t)n) + 1;

    /* compute base^5 */
    seq_t pow5 = 1;
    for (int i = 0; i < 5; ++i)
        pow5 = pow5 * base;

    /* add or subtract 1 according to parity of n */
    if ((n & 1) == 0)
        pow5 += 1;   /* even n: +1 */
    else
        pow5 -= 1;   /* odd n:  -1 */

    /* divide by 2 (exact division) */
    return pow5 / 2;
}
