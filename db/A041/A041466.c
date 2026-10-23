/*
 * A041466  Numerators of continued fraction convergents to sqrt(249).
 * https://oeis.org/A041466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 17107630*a(n-16) - a(n-32) This implementation computes
 *            the sequence via the convergents of the continued fraction of
 *            sqrt(249). The partial quotients after the initial term 15
 *            repeat with period 16: [1,3,1,1,5,1,3,10,3,1,5,1,1,3,1,30]
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 20, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const unsigned int period[] = {
    1, 3, 1, 1, 5, 1, 3, 10,
    3, 1, 5, 1, 1, 3, 1, 30
};
static const size_t period_len = sizeof period / sizeof period[0];

static seq_t *cache = NULL;   /* stored convergents */
static idx_t cache_len = 0;   /* number of terms currently stored */

seq_t a(idx_t n)
{
    /* initialise cache with the first convergent a(0)=15 */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)15;              /* a(0) */
    }

    /* extend cache to cover the requested index n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache;
        if (cache_len == 0) {
            new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        } else {
            new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        }
        if (!new_cache) return (seq_t)0;   /* allocation failure */
        cache = new_cache;

        /* compute missing convergents */
        for (idx_t k = cache_len; k <= n; ++k) {
            unsigned int coeff;
            if (k == 0) {
                /* already initialized */
                continue;
            } else {
                /* partial quotient for index k */
                coeff = period[(size_t)((k - 1) % period_len)];
            }

            if (k == 1) {
                /* use p_{-1} = 1 for the first recurrence step */
                cache[1] = (seq_t)coeff * cache[0] + (seq_t)1;
            } else {
                cache[k] = (seq_t)coeff * cache[k - 1] + cache[k - 2];
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
