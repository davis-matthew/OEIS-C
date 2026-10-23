/*
 * A129004  a(n) = (n^3 + n^2)*4^n.
 * https://oeis.org/A129004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n^2)*4^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* Compute 4^n = 1 << (2*n) iteratively */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
        pow <<= 2;          /* equivalent to multiplying by 4 each step */

    /* a(n) = n^2 * (n + 1) * 4^n */
    seq_t term = sn * sn;      /* n^2 */
    term *= (sn + 1);          /* n^2 * (n + 1) */
    term *= pow;               /* times 4^n */

    return term;
}
