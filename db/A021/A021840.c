/*
 * A021840  Decimal expansion of 1/836.
 * https://oeis.org/A021840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the (n + 1)-st decimal digit of 1/836
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = floor( ( ( ( (1*10) % 836 )*10 ) % 836 … *10 ) / 836 ) */
/*        after (n+1) steps of long division. */
seq_t a(idx_t n)
{
    /* The remainder starts with the numerator 1. */
    seq_t r = 1;
    seq_t digit = 0;

    /* Each decimal digit requires multiplying the remainder by 10, then
       taking the quotient and new remainder.  To obtain the (n+1)-th
       digit we perform n+1 iterations. */
    for (idx_t i = 0; i <= n; ++i)
    {
        r *= 10;           /* shift left by one decimal place   */
        digit = r / 836;   /* quotient gives the next digit      */
        r %= 836;          /* keep only the remainder             */
    }

    return digit;
}
