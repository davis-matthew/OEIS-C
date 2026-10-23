/*
 * A036482  a(n) = partition(11n+9) mod 11.
 * https://oeis.org/A036482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = partition(11*n + 9) mod 11
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for partition numbers modulo 11.
   p_cache[i] holds p(i) mod 11 for 0 <= i <= current maximum.
*/
static seq_t *p_cache = NULL;
static idx_t   p_cache_max = 0;

/* Forward declaration of helper that ensures the cache covers
   all indices up to N.
*/
static void ensure_partition_cache(idx_t N)
{
    if (N <= p_cache_max)
        return;                                      /* nothing to do */

    /* Allocate or grow cache */
    if (p_cache == NULL) {
        p_cache = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
        if (p_cache == NULL) {
            /* In an actual program we would handle allocation failure;
               for the purposes of the sequence function we simply
               abort. */
            abort();
        }
    } else {
        seq_t *tmp = (seq_t *)realloc(p_cache, (size_t)(N + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();
        p_cache = tmp;
    }

    /* Compute partition numbers modulo 11 incrementally. */
    if (p_cache_max == 0) {
        p_cache[0] = (seq_t)1;          /* p(0) = 1 */
    }

    for (idx_t i = p_cache_max + 1; i <= N; ++i) {
        int sum = 0;                    /* accumulate result modulo 11 */
        for (idx_t j = 1; ; ++j) {
            /* generalized pentagonal numbers:
               g1 = j*(3*j-1)/2   (k = +j)
               g2 = j*(3*j+1)/2   (k = -j)
            */
            idx_t g1 = j * (3 * j - 1) / 2;
            if (g1 > i) break;          /* no more terms for this i */
            int term1 = (int)p_cache[i - g1];
            if (j & 1) sum += term1;     /* j odd -> positive sign */
            else       sum -= term1;     /* j even -> negative sign */

            idx_t g2 = j * (3 * j + 1) / 2;
            if (g2 > i) continue;
            int term2 = (int)p_cache[i - g2];
            if (j & 1) sum += term2;
            else       sum -= term2;
        }
        /* Normalize modulo 11 into the range [0,10] */
        sum %= 11;
        if (sum < 0) sum += 11;
        p_cache[i] = (seq_t)sum;
    }

    p_cache_max = N;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t k = 11 * n + 9;          /* argument to partition function     */
    ensure_partition_cache(k);     /* make sure we have p(k) available   */
    return p_cache[k];             /* already mod 11                       */
}
