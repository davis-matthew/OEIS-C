/*
 * A021252  Decimal expansion of 1/248.
 * https://oeis.org/A021252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/248) mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th decimal digit of 1/248 (0‑indexed). */

seq_t a(idx_t n)
{
    seq_t rem   = 1;   /* current remainder in the long division */
    seq_t digit = 0;   /* digit computed at the current step */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t prod = rem * 10;   /* shift remainder left by one decimal position */
        digit = prod / 248;      /* next digit */
        rem   = prod % 248;      /* new remainder */
    }
    return digit;
}
