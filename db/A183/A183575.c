/*
 * A183575  a(n) = n - 1 + ceiling((n^2-2)/2); complement of A183574.
 * https://oeis.org/A183575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + ceil((n*n - 2)/2) Equivalent to: if n even: a(n)
 *            = (n*n + 2*n - 4)/2 if n odd : a(n) = (n*n + 2*n - 3)/2
 * Range:     u8 n <= 15, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t term = sq + 2 * (seq_t)n;           /* n^2 + 2*n */
    if (n & 1)                                 /* if n odd subtract 3 */
        term -= 3;
    else                                        /* if n even subtract 4 */
        term -= 4;
    return term / 2;                           /* divide by 2, truncates */
}
