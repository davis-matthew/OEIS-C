/*
 * A161214  a(n) = A161210(n)*3.
 * https://oeis.org/A161214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = 3*max(n*(n-1)/2, 7*n-14)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 3;
    if (n == 2)
        return 9;

    /* Compute choose(n,2) = n*(n-1)/2 */
    seq_t v1 = (seq_t)n * ((seq_t)n - 1) / 2;

    /* Compute 7*n-14; for n>=2 this is non‑negative */
    seq_t v2 = 7 * ((seq_t)n) - 14;

    seq_t m = v1 > v2 ? v1 : v2;
    return 3 * m;
}
