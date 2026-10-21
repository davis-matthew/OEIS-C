/*
 * A227918  Sum over all permutations beginning and ending with ascents, and
 *          without double ascents on n elements and each permutation
 *          contributes 2 to the power of the number of double descents.
 * https://oeis.org/A227918
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + 1 + 4*(-1)^n (a(2)=1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses memoization; works for any n >= 2. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* number of slots allocated */
static idx_t  last_computed = 1;  /* largest n whose a(n) is stored */

seq_t a(idx_t n)
{
    /* Ignore n < 2 – problem statement requires n >= 2. */
    if (n < 2) return 0;

    /* Ensure the cache is large enough. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failed – give up */
        cache = tmp;
        /* Zero the newly allocated part. */
        memset((void *)(cache + cache_cap), 0,
               (new_cap - cache_cap) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise a(2) if it hasn't been yet. */
    if (cache[2] == 0) {
        cache[2] = 1;
        last_computed = 2;
    }

    /* Compute values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)i * cache[i - 1];
        if (i & 1UL)        /* odd i: subtract 3 */
            val -= (seq_t)3;
        else                /* even i: add 5 */
            val += (seq_t)5;
        cache[i] = val;
    }
    last_computed = n;

    return cache[n];
}
