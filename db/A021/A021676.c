/*
 * A021676  Decimal expansion of 1/672.
 * https://oeis.org/A021676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of 1/672.  a(n) is the (n+1)-th digit after the decimal point. */
seq_t a(idx_t n)
{
    const seq_t den = 672;
    seq_t rem = 1;          /* current remainder, always < den */
    seq_t digit = 0;

    for (idx_t i = 0; ; ++i)
    {
        rem *= 10;          /* bring next decimal digit into place */
        digit = rem / den;  /* integer part -> next decimal digit */
        rem = rem % den;    /* new remainder */
        if (i == n)
            return digit;   /* return the desired (n+1)-th digit */
    }
}
