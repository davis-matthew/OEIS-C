/*
 * A021709  Decimal expansion of 1/705.
 * https://oeis.org/A021709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th digit (0‑based) of the decimal expansion of
 *            1/705 after the decimal point. The sequence is the digits
 *            0,0,1,4,1,8,4,3,9,7,1,6 …
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    uint64_t r = 1;            /* initial remainder (1) */
    seq_t digit = 0;

    /* Perform long division 10 times for each digit.
       For n=0 we need the first digit, so run the loop n+1 times. */
    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10;               /* bring down next digit */
        digit = (seq_t)(r / 705);
        r = r % 705;
    }
    return digit;
}
