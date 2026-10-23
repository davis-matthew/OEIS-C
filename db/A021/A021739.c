/*
 * A021739  Decimal expansion of 1/735.
 * https://oeis.org/A021739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of 1/735 (computed by long division) */
seq_t a(idx_t n)
{
    idx_t rem = 1;          /* current remainder */
    seq_t digit = 0;        /* current digit */

    for (idx_t i = 0; i <= n; ++i)
    {
        rem = rem * 10;                 /* shift left one decimal place */
        digit = (seq_t)(rem / 735);     /* digit at this position */
        rem = rem % 735;                /* new remainder for next step */
    }
    return digit;
}
