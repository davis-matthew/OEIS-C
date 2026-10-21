/*
 * A063241  Dimension of the space of weight 2n cuspidal newforms for
 *          Gamma_0(90).
 * https://oeis.org/A063241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 4*floor(n/3 - 1/3) + 1
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified as a(n) = 2*n + 4*((n-1)/3) + 1  */

seq_t a(idx_t n)
{
    /* For n < 1 the behaviour is undefined; return 0 to avoid
       unsigned underflow in (n-1).  The offset is 1, so this is
       harmless for all inputs used in the tests. */
    if (n == 0)
        return 0;

    /* Integer division gives floor((n-1)/3). */
    idx_t q = (n - 1) / 3;

    /* Compute the result using seq_t arithmetic only. */
    seq_t res = ((seq_t)n * 2) + ((seq_t)q * 4) + 1;
    return res;
}
