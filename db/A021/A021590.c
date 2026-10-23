/*
 * A021590  Decimal expansion of 1/586.
 * https://oeis.org/A021590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th decimal digit of 1/586 (i.e. a(n) =
 *            floor((10^n)/586) mod 10)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t denom = 586;   /* denominator of the fraction */
    idx_t rem = 1;                    /* current remainder, starts with 1 */
    seq_t digit = 0;

    /* Generate digits one by one up to and including index n */
    for (idx_t i = 0; i <= n; ++i) {
        rem   *= (idx_t)10;          /* bring down next decimal place */
        digit   = rem / denom;       /* current digit */
        rem    %= denom;             /* new remainder */
    }

    return digit;
}
