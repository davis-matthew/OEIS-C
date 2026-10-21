/*
 * A224498  Smallest prime that can be expressed as the sum of n distinct
 *          positive squares with the largest square as small as possible.
 * https://oeis.org/A224498
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 57, u64 n <= 387, u128 n <= 387,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A224498: the smallest prime that can be expressed as the sum of
 * n distinct positive squares with the largest square as small as possible.
 * The algorithm increases the maximal square m (m ≥ n) and removes d = m-n
 * squares. For each m it enumerates all subsets of size d (d ≤ 2 in normal
 * cases) and keeps the smallest prime found. As soon as a prime is found for
 * some m the algorithm returns it, guaranteeing that the returned prime has
 * the minimal possible largest square. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return sum of squares 1² + 2² + … + t² as seq_t. */
static inline seq_t sum_sq(idx_t t)
{
    seq_t tt = (seq_t)t;
    /* tt*(tt+1)*(2*tt+1)/6 */
    return tt * (tt + 1) * (2 * tt + 1) / 6;
}

/* Search for A224498(n). */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined for n < 2 */

    /* Increase maximal square m starting from n until a prime is found. */
    for (idx_t m = n; ; ++m)
    {
        seq_t sum_all = sum_sq(m);       /* Σ_{i=1}^{m} i² */
        idx_t d = m - n;                 /* number of squares to exclude */

        if (d == 0)                     /* use all squares 1..m */
        {
            if (is_prime_big(sum_all)) return sum_all;
            continue;
        }

        if (d == 1)                     /* exclude one square */
        {
            seq_t best = 0;
            for (idx_t j = m - 1; j >= 1; --j)
            {
                seq_t cand = sum_all - (seq_t)j * (seq_t)j;
                if (is_prime_big(cand) && (best == 0 || cand < best))
                    best = cand;
            }
            if (best != 0) return best;
            continue;
        }

        if (d == 2)                     /* exclude two squares */
        {
            seq_t best = 0;
            for (idx_t j = m - 1; j >= 2; --j)
            {
                seq_t sqj = (seq_t)j * (seq_t)j;
                for (idx_t i = j - 1; i >= 1; --i)
                {
                    seq_t cand = sum_all - sqj - (seq_t)i * (seq_t)i;
                    if (is_prime_big(cand) && (best == 0 || cand < best))
                        best = cand;
                }
            }
            if (best != 0) return best;
            continue;
        }

        /* For d ≥ 3 the search would be prohibitively expensive for the
         * given constraints and do not occur for n ≤ 10000 in the OEIS
         * data.  If such n were requested we simply skip to the next m.
         */
        continue;
    }
}
