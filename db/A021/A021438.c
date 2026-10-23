/*
 * A021438  Decimal expansion of 1/434.
 * https://oeis.org/A021438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/434) mod 10 (the n-th digit of 1/434)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    uint64_t rem = 1;          /* remainder after each step, always < 434          */
    seq_t     digit = 0;       /* holds the current decimal digit (0..9)         */

    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10ULL;                        /* bring down next digit of the decimal     */
        digit = (seq_t)(rem / 434ULL);       /* integer part of the division              */
        rem   = rem % 434ULL;                /* new remainder                            */
    }
    return digit;
}
