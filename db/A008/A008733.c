/*
 * A008733  Molien series for 3-dimensional group [2+, n] = 2*(n/2).
 * https://oeis.org/A008733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 5*n + 13 + 3*(n+1)*(-1)^n)/16)
 * Range:     u8 n <= 11, u16 n <= 290, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                /* cast index to seq_t */
    seq_t term = nn * nn + 5 * nn + (seq_t)13; /* n^2 + 5n + 13 */

    if ((n & 1ULL) == 0)                /* even n: add 3*(n+1) */
        term += 3 * (nn + 1);
    else                                /* odd n: subtract 3*(n+1) */
        term -= 3 * (nn + 1);

    return term / (seq_t)16;            /* integer division = floor for non‑negative */
}
