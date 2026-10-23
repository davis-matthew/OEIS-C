/*
 * A021711  Decimal expansion of 1/707.
 * https://oeis.org/A021711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th decimal digit of 1/707 after the decimal point
 *            (0‑indexed)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses long‑division to generate the digits. */
seq_t a(idx_t n)
{
    seq_t r = 1;                /* current remainder in the division 1/707 */
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t digit = (r * 10) / 707; /* next decimal digit */
        r = (r * 10) % 707;          /* update remainder */
        if (i == n) return digit;    /* return the requested digit */
    }
    return 0; /* unreachable, but satisfies all control paths */
}
