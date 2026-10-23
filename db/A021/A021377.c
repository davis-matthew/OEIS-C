/*
 * A021377  Decimal expansion of 1/373.
 * https://oeis.org/A021377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1) / 373⌋ mod 10 (nth decimal digit of 1/373)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;      /* current remainder (starts as the numerator) */
    seq_t digit = 0;  /* digit to be returned after the loop */

    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;                 /* bring down the next decimal place */
        digit = r / 373;         /* digit in this place */
        r %= 373;                /* new remainder */
    }
    return digit;
}
