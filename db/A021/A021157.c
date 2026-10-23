/*
 * A021157  Decimal expansion of 1/153.
 * https://oeis.org/A021157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n‑th digit after the decimal point in the expansion
 *            of 1/153. The sequence is periodic with period 16:
 *            0,0,6,5,3,5,9,4,7,7,1,2,4,1,8,3
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = (idx_t)1;          /* current remainder */
    seq_t digit = (seq_t)0;      /* digit to return */
    idx_t i;

    for (i = (idx_t)0; i <= n; ++i)
    {
        r = r * (idx_t)10;                 /* bring down next decimal digit */
        digit = (seq_t)(r / (idx_t)153);   /* integer division gives digit */
        r = r % (idx_t)153;                /* new remainder for next step   */
    }

    return digit;
}
