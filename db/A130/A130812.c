/*
 * A130812  If X_1,...,X_n is a partition of a 2n-set X into 2-blocks then
 *          a(n) is equal to the number of 6-subsets of X containing none of
 *          X_i, (i=1,...n).
 * https://oeis.org/A130812
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64 * binom(n,6) for n >= 6
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 62, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset */
    if (n < 6) return 0;

    seq_t result = 1;
    const idx_t k = 6;

    /* Compute binomial coefficient (n choose 6) iteratively */
    for (idx_t i = 1; i <= k; ++i) {
        /* Use 128‑bit intermediate to prevent overflow */
        __int128 temp = (__int128)result * (n - k + i);
        temp /= i;
        result = (seq_t)temp;
    }

    /* Multiply by 64 */
    __int128 temp = (__int128)result * 64;
    result = (seq_t)temp;

    return result;
}
