/*
 * A081360  Expansion of q^(-1/24) (m (1-m) / 16)^(1/24) in powers of q, where
 *          m = k^2 is the parameter and q is the nome for Jacobian elliptic
 *          functions.
 * https://oeis.org/A081360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, changed
 * Formula:   a(n) = coefficient of x^n in Π_{k≥1}(1+(-1)^k x^k) (Number of
 *            partitions of n into distinct parts, with sign given by the
 *            parity of the number of odd parts) Computation is performed once
 *            up to a maximum of 10 000 (or the first requested n if larger);
 *            a(n) is then returned from the cache
 * Range:     i8 n <= 24, i16 n <= 70, i32 n <= 225, i64 n <= 769,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array holding a(0)…a(max_cached) */
static idx_t  cache_size = 0;        /* number of elements currently stored (== max_cached + 1) */

/* Build the cache up to “up_to” (inclusive).  All values are recomputed. */
static void build_cache(idx_t up_to)
{
    seq_t *new_cache = (seq_t*)realloc(cache, (up_to + 1) * sizeof(seq_t));
    if (!new_cache) { /* memory exhausted – abort */
        return;
    }
    cache = new_cache;

    /* initialise */
    for (idx_t i = 0; i <= up_to; ++i) cache[i] = 0;
    cache[0] = 1;   /* empty partition */

    /* dynamic programming: 0/1 knapsack with sign depending on part parity */
    for (idx_t part = 1; part <= up_to; ++part) {
        if ((part & 1) == 0) {               /* even part – sign +1 */
            for (idx_t sum = up_to; sum >= part; --sum)
                cache[sum] = cache[sum] + cache[sum - part];
        } else {                              /* odd part – sign –1 */
            for (idx_t sum = up_to; sum >= part; --sum)
                cache[sum] = cache[sum] - cache[sum - part];
        }
    }
    cache_size = up_to + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Compute up to a reasonable upper bound once.  The sequence is known up to at least 10 000. */
    if (cache == NULL) {
        idx_t init = (n > 10000) ? n : 10000;
        build_cache(init);
    }

    /* If a larger n is requested, extend the cache accordingly. */
    if (n >= cache_size) {
        build_cache(n);
    }

    return cache[n];
}
