/*
 * A129502  For n=2^k, a(n) = binomial(k + 2, 2), else 0.
 * https://oeis.org/A129502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = binomial(k+2,2) if n = 2^k else 0
 * Range:     u8 n <= 1024, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Any n that is not a power of two yields 0 */
    if (n == 0) return 0;             /* not part of OEIS offset but harmless */
    if ((n & (n - 1)) != 0) return 0;

    /* Determine k such that n = 2^k. */
    idx_t tmp = n;
    int k = 0;
    while ((tmp & 1) == 0)
    {
        tmp >>= 1;
        ++k;
    }

    /* Compute binomial(k+2, 2) = (k+2)*(k+1)/2 in seq_t */
    seq_t a1 = (seq_t)(k + 2);
    seq_t a2 = (seq_t)(k + 1);
    return a1 * a2 / 2;
}
