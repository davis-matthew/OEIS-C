/*
 * A021662  Decimal expansion of 1/658.
 * https://oeis.org/A021662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the (n+1)th decimal digit of 1/658 (0‑indexed)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1/658 has repeating decimal digits.  We compute its n-th digit by long
       division: start with remainder 1, then for each step multiply by 10,
       take the integer division by 658 to get the next digit, and keep the
       remainder for the next step.  */
    const seq_t divisor = (seq_t)658;
    seq_t r = (seq_t)1;     /* remainder before any digit */
    seq_t digit = 0;        /* will hold the digit for the current step */

    for (idx_t i = 0; i <= n; ++i) {
        r = r * (seq_t)10;          /* bring next decimal place */
        digit = r / divisor;        /* integer part = next digit */
        r = r % divisor;            /* keep remainder */
    }
    return digit;
}
