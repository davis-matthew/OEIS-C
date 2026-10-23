/*
 * A017225  a(n) = (9*n + 5)^5.
 * https://oeis.org/A017225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n+5)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 8, u64 n <= 791, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (9*n + 5) in seq_t type */
    seq_t t = (seq_t)9 * (seq_t)n; /* 9*n */
    t += (seq_t)5;                 /* +5 */

    /* Compute t^5 by repeated multiplication */
    seq_t res = t;                /* t¹ */
    res *= t;                     /* t² */
    res *= t;                     /* t³ */
    res *= t;                     /* t⁴ */
    res *= t;                     /* t⁵ */

    return res;
}
