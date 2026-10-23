/*
 * A021900  Decimal expansion of 1/896.
 * https://oeis.org/A021900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth decimal digit of 1/896 (0‑indexed)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t rem = 1;          /* current remainder before producing a digit   */
    seq_t digit = 0;        /* last produced digit                         */
    idx_t i;

    /* Produce digits for positions 0 … n, so the digit computed in the
       final iteration is a(n).                                            */
    for (i = 0; i <= n; ++i) {
        rem *= 10;                      /* shift remainder left one decimal digit */
        digit = (seq_t)(rem / 896);      /* next decimal digit (floor)           */
        rem = rem % 896;                 /* new remainder for the next digit     */
    }

    return digit;
}
