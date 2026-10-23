/*
 * A021678  Decimal expansion of 1/674.
 * https://oeis.org/A021678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th decimal digit of 1/674, with a(0) being the
 *            first digit after the decimal point
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The remainder after extracting the first n digits. */
    seq_t rem = 1;        /* initially 1 = 1 / 674 */

    /* Extract the digits one by one. */
    for (idx_t i = 0; i < n; ++i)
    {
        rem *= 10;                  /* bring down the next decimal place */
        rem = rem % 674;            /* keep only the remainder  */
    }

    /* Compute the (n+1)-th digit (which is a(n)). */
    rem *= 10;
    seq_t digit = rem / 674;        /* integer division yields the digit 0–9 */

    return digit;
}
