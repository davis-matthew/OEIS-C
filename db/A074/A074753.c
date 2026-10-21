/*
 * A074753  Number of integers k such that sigma(k) < n.
 * https://oeis.org/A074753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k : sigma(k) < n} (offset 1)
 * Range:     u8 n <= 380, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n <= 1)
        return 0;

    /* We need sigma values up to k = n-2, but always at least 1 for k=1 */
    idx_t need = n - 2;
    if (need < 1)
        need = 1;

    static seq_t *sigma = NULL;     /* sigma[k] = sum of divisors of k */
    static idx_t limit = 0;         /* currently computed maximum k */

    /* Extend the sigma array if required */
    if (need > limit) {
        /* Allocate / reallocate memory */
        seq_t *new_sigma = (seq_t *)realloc(sigma, sizeof(seq_t) * (need + 1));
        if (!new_sigma)
            return 0; /* Allocation failed: return 0 as a safe fallback */
        sigma = new_sigma;

        /* Zero new elements */
        for (idx_t i = limit + 1; i <= need; ++i)
            sigma[i] = 0;

        /* Add contributions from existing i (1..limit) to new indices */
        for (idx_t i = 1; i <= limit; ++i) {
            /* First multiple > limit */
            idx_t start = ((limit + i) / i) * i; /* ceil((limit+1)/i)*i */
            for (idx_t j = start; j <= need; j += i)
                sigma[j] += i;
        }

        /* Add contributions from new i (limit+1..need) */
        for (idx_t i = limit + 1; i <= need; ++i) {
            for (idx_t j = i; j <= need; j += i)
                sigma[j] += i;
        }

        limit = need;
    }

    /* Count k such that sigma(k) < n */
    seq_t count = 0;
    seq_t sn = (seq_t)n;
    for (idx_t k = 1; k <= need; ++k) {
        if (sigma[k] < sn)
            ++count;
    }

    return count;
}
