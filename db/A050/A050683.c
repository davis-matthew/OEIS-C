/*
 * A050683  Number of nonzero palindromes of length n.
 * https://oeis.org/A050683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(n) = 9*10^floor((n-1)/2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n == 0 we return 0
       because the OEIS offset is 1 and behavior below the offset
       does not matter. */
    if (n == 0)
        return 0;

    /* exponent floor((n-1)/2) */
    idx_t exp = (n - 1) / 2;

    seq_t result = (seq_t)9;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)10;

    return result;
}
