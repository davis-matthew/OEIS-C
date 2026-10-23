/*
 * A035642  Number of partitions of n into parts 6k+1 and 6k+2 with at least
 *          one part of each type.
 * https://oeis.org/A035642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_all(n) – p1(n) – p2(n) where p_all(n) = #partitions of
 *            n into parts ≡ 1 or 2 (mod 6) p1(n) = #partitions of n using
 *            only parts ≡ 1 (mod 6) p2(n) = #partitions of n using only parts
 *            ≡ 2 (mod 6) (All partitions are unordered and parts may repeat.)
 *            This follows from the product of generatin...
 * Range:     u8 n <= 34, u16 n <= 98, u32 n <= 316, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build system. */

static seq_t *cache = NULL;   /* cache[n-1] holds a(n) for 1≤n≤cached_n */
static idx_t cached_n = 0;    /* largest n for which cache is valid */

/* Forward declaration of internal helper */
static void compute_cache(idx_t upto);

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; undefined for n=0, return 0 */
        return (seq_t)0;
    if (n > cached_n)
        compute_cache(n);

    return cache[n - 1];
}

/* Compute all values a(1)…a(upto) and store them in cache. */
static void compute_cache(idx_t upto)
{
    /* Allocate space for dp arrays and the new cache */
    seq_t *dp_all = (seq_t*)calloc(upto + 1, sizeof(seq_t));
    seq_t *dp_1   = (seq_t*)calloc(upto + 1, sizeof(seq_t));
    seq_t *dp_2   = (seq_t*)calloc(upto + 1, sizeof(seq_t));
    if (!dp_all || !dp_1 || !dp_2) {
        /* Out of memory: abort correctly */
        abort();
    }

    dp_all[0] = 1;
    dp_1[0]   = 1;
    dp_2[0]   = 1;

    /* Process parts congruent 1 mod 6 */
    for (idx_t part = 1; part <= upto; part += 6) {
        for (idx_t i = part; i <= upto; ++i) {
            dp_all[i] += dp_all[i - part];
            dp_1[i]   += dp_1[i - part];
        }
    }
    /* Process parts congruent 2 mod 6 */
    for (idx_t part = 2; part <= upto; part += 6) {
        for (idx_t i = part; i <= upto; ++i) {
            dp_all[i] += dp_all[i - part];
            dp_2[i]   += dp_2[i - part];
        }
    }

    /* Allocate or grow the cache */
    cache = (seq_t*)realloc(cache, upto * sizeof(seq_t));
    if (!cache) abort();

    for (idx_t i = 1; i <= upto; ++i) {
        seq_t val = dp_all[i];
        val -= dp_1[i];
        val -= dp_2[i];
        cache[i - 1] = val;
    }

    /* Update state */
    cached_n = upto;

    /* Free temporary arrays */
    free(dp_all);
    free(dp_1);
    free(dp_2);
}
