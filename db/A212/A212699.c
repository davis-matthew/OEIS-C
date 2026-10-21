/*
 * A212699  Main transitions in systems of n particles with spin 2.
 * https://oeis.org/A212699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(b-1)*b^(n-1) where b=5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = 4*n*5^(n-1) */

seq_t a(idx_t n)
{
    /* Compute 5^(n-1) iteratively */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        res *= (seq_t)5;

    /* Multiply by 4*n */
    res *= (seq_t)4;
    res *= (seq_t)n;

    return res;
}
