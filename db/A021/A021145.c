/*
 * A021145  Decimal expansion of 1/141.
 * https://oeis.org/A021145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A021145: decimal expansion of 1/141.
 * The digit a(n) is the (n+1)-th digit after the decimal point of 1/141.
 */
seq_t a(idx_t n)
{
    seq_t r = 1;          /* current remainder of the division 1 mod 141 */
    seq_t digit = 0;      /* digit to be returned */

    /* perform (n+1) iterations to obtain the digit at position n */
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;                     /* bring down next decimal digit */
        digit = r / (seq_t)141;      /* integer part is the next digit */
        r = r % (seq_t)141;          /* remainder for next step */
    }
    return digit;
}
