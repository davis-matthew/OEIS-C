/*
 * A157825  a(n) = 1728000*n - 1620240.
 * https://oeis.org/A157825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1728000*n - 1620240
 * Range:     u8 none, u16 none, u32 n <= 2486, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A157825. */
seq_t a(idx_t n)
{
    /* Using the linear formula directly. */
    return (seq_t)1728000 * n - (seq_t)1620240;
}
