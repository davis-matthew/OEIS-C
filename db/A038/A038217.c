/*
 * A038217  Triangle whose (i,j)-th entry is binomial(i,j)*2^(i-j)*11^j.
 * https://oeis.org/A038217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = T(i,j) where T(i,j) = binomial(i,j) * 2^(i-j) * 11^j,
 *            with i the unique integer satisfying i*(i+1)/2 <= n <
 *            (i+1)*(i+2)/2 and j = n - i*(i+1)/2
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static seq_t binomial_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t t = 1; t <= k; ++t)
        r = r * (seq_t)(n - k + t) / (seq_t)t;
    return r;
}

seq_t a(idx_t n)
{
    /* Find row i such that total elements up to row i-1 <= n */
    idx_t i = 0;
    while ((unsigned __int128)(i + 1) * (i + 2) / 2 <= n) ++i;

    /* Column j within that row */
    idx_t j = (idx_t)((unsigned __int128)n - (unsigned __int128)i * (i + 1) / 2);

    /* Compute the entry T(i,j) */
    seq_t val = binomial_idx(i, j);

    /* × 2^(i-j) */
    for (idx_t t = 0; t < i - j; ++t)
        val *= 2;

    /* × 11^j */
    for (idx_t t = 0; t < j; ++t)
        val *= 11;

    return val;
}
