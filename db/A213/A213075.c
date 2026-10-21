/*
 * A213075  Second diagonal of A213074.
 * https://oeis.org/A213075
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = p(n) - floor((n+1)/2) - 1 where p(n) is the partition
 *            function (number of unrestricted partitions of n)
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* External linkage: the required function */
seq_t a(idx_t n)
{
    /* cache for partition numbers */
    static seq_t *p_cache = NULL;
    static idx_t cache_n = 0;

    /* Ensure we have computed p(k) for all k <= n */
    if (n > cache_n) {
        /* Allocate or expand the cache */
        seq_t *new_cache = (seq_t *)realloc(p_cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) return 0;  /* fallback on allocation failure */
        p_cache = new_cache;

        /* Initialize p(0) if not already */
        if (cache_n == 0) {
            p_cache[0] = 1;
            cache_n = 0;
        }

        /* Compute partition numbers from cache_n+1 up to n */
        for (idx_t k = cache_n + 1; k <= n; ++k) {
            seq_t sum = 0;
            for (idx_t m = 1; ; ++m) {
                idx_t pent1 = m * (3 * m - 1) / 2;
                idx_t pent2 = m * (3 * m + 1) / 2;
                int sign = (m & 1) ? 1 : -1;  /* alternate +, - */

                if (k >= pent1) {
                    if (sign > 0) sum += p_cache[k - pent1];
                    else          sum -= p_cache[k - pent1];
                }
                if (k >= pent2) {
                    if (sign > 0) sum += p_cache[k - pent2];
                    else          sum -= p_cache[k - pent2];
                }
                if (k < pent1 && k < pent2) break;
            }
            p_cache[k] = sum;
        }
        cache_n = n;
    }

    /* Compute a(n) = p(n) - floor((n+1)/2) - 1 */
    seq_t part = p_cache[n];
    seq_t sub = (seq_t)((n + 1) / 2);
    return part - sub - 1;
}
