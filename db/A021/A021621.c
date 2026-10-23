/*
 * A021621  Decimal expansion of 1/617.
 * https://oeis.org/A021621
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

/*  Computes the n‑th decimal digit of 1/617 by simulating long division.  */
seq_t a(idx_t n)
{
    /* Remainder before generating the next digit.  */
    idx_t rem = 1;
    seq_t digit = 0;

    /* Generate digits one by one up to index n.  */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10;                      /* Bring next digit in.      */
        digit = (seq_t)(rem / 617);     /* Current decimal digit.   */
        rem   = rem % 617;              /* New remainder.           */
    }
    return digit;
}
