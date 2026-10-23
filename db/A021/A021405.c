/*
 * A021405  Decimal expansion of 1/401.
 * https://oeis.org/A021405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit at position n (0‑indexed) of the decimal expansion
 *            of 1/401. Equivalent to: a(n) = floor((10 * (10^n mod 401)) /
 *            401)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;       /* initial remainder: 1/401 */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10;             /* bring down next decimal digit */
        digit = r / 401;        /* integer part of this step */
        r = r % 401;            /* new remainder for the next step */
    }
    return digit;    /* digit at decimal position n (0‑indexed) */
}
