/*
 * A021288  Decimal expansion of 1/284.
 * https://oeis.org/A021288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th digit of the decimal expansion of 1/284 (after the point) */
seq_t a(idx_t n)
{
    /* Local remainder for long division of 1 by 284 */
    uint64_t rem = 1;        /* 1/284 */
    seq_t digit = 0;

    /* Compute digits up to position n (inclusive) */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10ULL;                     /* bring next decimal digit */
        digit = (seq_t)(rem / 284ULL);    /* integer part gives the digit */
        rem -= (uint64_t)digit * 284ULL;  /* keep the new remainder */
    }
    return digit;
}
