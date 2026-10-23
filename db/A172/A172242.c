/*
 * A172242  Number of 10-D hypercubes in an n-dimensional hypercube.
 * https://oeis.org/A172242
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,10) * 2^(n-10)
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10)          /* sequence not defined below the offset */
        return (seq_t)0;

    seq_t res = 1;

    /* compute binomial(n,10) multiplicatively */
    for (idx_t i = 1; i <= 10; ++i) {
        res = res * (n - 10 + i) / i;
    }

    /* multiply by 2^(n-10) */
    for (idx_t i = 10; i < n; ++i) {
        res = res * 2;
    }

    return res;
}
