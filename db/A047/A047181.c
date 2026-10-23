/*
 * A047181  Number of nonempty subsets of {1,2,...,n} in which exactly 5/6 of
 *          the elements are <= (n-1)/2.
 * https://oeis.org/A047181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * sum_{k>=1, 6k <= n} binomial(floor((n-1)/2), 5k) *
 *            binomial(n - floor((n-1)/2), k)
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sum counts subsets of size 6k with exactly 5k small (≤(n-1)/2) and k large elements. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Count how many elements are in the “small” and “large” halves. */
    idx_t s = (n - 1) / 2;          /* number of elements ≤ (n-1)/2 */
    idx_t l = n - s;                /* number of elements > (n-1)/2 */

    seq_t total = 0;

    /* Iterate over possible subset sizes 6k (k ≥ 1).  The condition 6k ≤ n
     * ensures m=6k fits inside {1,…,n}. */
    for (idx_t k = 1; 6 * k <= n; ++k) {
        /* Need at least 5k small elements and k large elements available. */
        if (s < 5 * k || l < k)
            continue;

        seq_t small   = binomial(s, 5 * k);   /* choose 5k small elements */
        seq_t large   = binomial(l, k);       /* choose k large elements */
        seq_t prod    = small * large;        /* product of the two choices */

        total += prod;                       /* accumulate the contribution */
    }

    return total;
}
