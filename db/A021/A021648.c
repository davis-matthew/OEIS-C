/*
 * A021648  Decimal expansion of 1/644.
 * https://oeis.org/A021648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th decimal digit (0‑indexed) of 1/644 after the decimal
 *            point. The digit is obtained by repeated long‑division: starting
 *            with remainder r = 1, each step r *= 10 digit = r / 644 r = r %
 *            644 The sequence returns the digit from step n
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;                /* remainder, 1 / 644 */
    for (idx_t i = 0; ; ++i) {
        r *= 10;                /* shift left one decimal place */
        seq_t d = r / 644;      /* next decimal digit */
        r = r % 644;            /* new remainder */
        if (i == n)
            return d;           /* return the required digit */
    }
}
