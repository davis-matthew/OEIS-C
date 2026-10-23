/*
 * A021505  Decimal expansion of 1/501.
 * https://oeis.org/A021505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth digit after the decimal point of 1/501 (OEIS
 *            A021505). The decimal expansion of 1/501 is 0.001996007984...
 *            This implementation performs the standard long‑division digit
 *            extraction
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;          /* current remainder (initially 1 for 1/501) */
    seq_t d = 0;          /* digit to be returned */

    /* For each position 0 … n, multiply by 10 and obtain the next digit. */
    for (idx_t i = 0; i <= n; ++i)
    {
        r *= 10;           /* bring down next 0 (multiply by 10) */
        d = r / 501;       /* digit = quotient of division by 501 */
        r %= 501;          /* remainder for next step */
    }
    return d;
}
