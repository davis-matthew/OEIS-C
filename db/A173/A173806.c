/*
 * A173806  a(n) = (7*10^n - 61)/9 for n > 0.
 * https://oeis.org/A173806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = 10*a(n-1) + 61 for n > 1 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1, value unused */

    seq_t cur = 1;                     /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
        cur = cur * 10 + 61;           /* exact arithmetic for any seq_t */

    return cur;
}
