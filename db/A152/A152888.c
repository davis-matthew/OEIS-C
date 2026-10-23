/*
 * A152888  Partial sums of length of terms in A081368 where A081368(1) is set
 *          to 0.
 * https://oeis.org/A152888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = n*(n+1)/2, except a(n) = n*(n+1)/2 - 1 for n < 6 or n >
 *            11. This reproduces the partial sums of lengths of A081368 with
 *            A081368(1) set to 0
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the triangular number n*(n+1)/2 in the target integer type. */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;

    /* For n = 6…11 the sequence coincides with the triangular numbers.
       Otherwise we subtract one. */
    if (n < 6 || n > 11)
        return T - 1;
    return T;
}
