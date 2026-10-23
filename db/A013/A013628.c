/*
 * A013628  Triangle of coefficients in expansion of (4 + 5*x)^n.
 * https://oeis.org/A013628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = binomial(m,k) * 4^(m-k) * 5^k where m =
 *            floor((sqrt(8*n+1)-1)/2) (row index) k = n - m*(m+1)/2 (column
 *            index) The sequence is the flattened coefficients of (4 + 5*x)^m
 * Range:     u8 n <= 7, u16 n <= 22, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2 */
    idx_t m = 0;
    for (;; ++m) {
        unsigned __int128 s = (unsigned __int128)(m + 1) * (m + 2) / 2;
        if ((unsigned __int128)n < s)
            break;
    }

    /* Index of first element of row m */
    unsigned __int128 start = (unsigned __int128)m * (m + 1) / 2;

    /* Column index k (0 <= k <= m) */
    idx_t k = (idx_t)((unsigned __int128)n - start);

    /* Binomial coefficient */
    seq_t val = binomial(m, k);

    /* Multiply by 4^(m-k) */
    for (idx_t i = 0; i < m - k; ++i)
        val *= (seq_t)4;

    /* Multiply by 5^k */
    for (idx_t i = 0; i < k; ++i)
        val *= (seq_t)5;

    return val;
}
