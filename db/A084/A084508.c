/*
 * A084508  Partial sums of A084509. Positions of ones in the first
 *          differences of A084506.
 * https://oeis.org/A084508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n for n < 2, a(n) = 2^(2*n-3) + 1 for n >= 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)n;

    idx_t exp = 2*n - 3;   /* exponent 2*n-3 */
    seq_t res = (seq_t)1;  /* start with 1 */

    /* multiply by 2 in a loop so we avoid undefined shifts on large exponents */
    for (idx_t i = 0; i < exp; ++i)
        res *= 2;

    res += 1;
    return res;
}
