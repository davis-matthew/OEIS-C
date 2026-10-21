/*
 * A052810  a(n) = 1 + (number of partitions of n, n>0).
 * https://oeis.org/A052810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// OEIS A052810: a(n) = 1 + number of partitions of n (for n>0), a(0)=1
// Uses the generalized pentagonal number theorem.
// The cache is static so calls in any order are supported.
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: partition numbers for indices < cache_size */
    static seq_t *partition_cache = NULL;
    static idx_t cache_size = 0;   /* current cache holds 0 .. cache_size-1 */

    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    /* grow cache to include n */
    if (n >= cache_size) {
        /* allocate or enlarge the array (realloc handles null pointer) */
        seq_t *tmp = (seq_t *)realloc(partition_cache,
                                      sizeof(seq_t) * (n + 1));
        if (!tmp) return (seq_t)0;           /* allocation failure, return 0 */
        partition_cache = tmp;

        /* if the cache is just being created, initialise index 0 */
        if (cache_size == 0) {
            partition_cache[0] = (seq_t)1;
            cache_size = 1;
        }

        /* compute partition numbers for new indices */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; ; ++k) {
                idx_t pent1 = k * (3 * k - 1) / 2;
                if (pent1 > i) break;                 /* no more contributors */
                if (k & 1) sum += partition_cache[i - pent1];
                else       sum -= partition_cache[i - pent1];

                idx_t pent2 = k * (3 * k + 1) / 2;
                if (pent2 > i) continue;
                if (k & 1) sum += partition_cache[i - pent2];
                else       sum -= partition_cache[i - pent2];
            }
            partition_cache[i] = sum;
        }
        cache_size = n + 1;
    }

    return partition_cache[n] + (seq_t)1;  /* a(n) = p(n) + 1 */
}
