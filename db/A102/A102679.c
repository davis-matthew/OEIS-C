/*
 * A102679  Number of digits >= 7 in decimal representation of n.
 * https://oeis.org/A102679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of decimal digits of n that are >= 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;          /* count of qualifying digits */
    idx_t t   = n;          /* work on a copy of n */

    while (t != 0) {
        idx_t d = t % 10;   /* current least‑significant decimal digit */
        if (d >= 7)
            ++cnt;          /* increment count if digit is 7, 8 or 9 */
        t /= 10;           /* move to the next digit */
    }

    return cnt;            /* value fits in seq_t */
}
