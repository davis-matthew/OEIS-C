/*
 * A123955  Expansion of g.f.: x^5/( (1-3*x) * (1-2*x) * (1-4*x) *
 *          (1-6*x+6*x^2) ).
 * https://oeis.org/A123955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n−1) − 86*a(n−2) + 234*a(n−3) − 300*a(n−4) +
 *            144*a(n−5) Initial values: a(1)=0, a(2)=0, a(3)=0, a(4)=0,
 *            a(5)=1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 17, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Edge case: indices < 1 are undefined, return 0 */
    if (n < 1)
        return 0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* allocated length (indices 0..len-1) */
    static idx_t last_computed = 0;     /* highest index computed so far */

    /* Ensure the cache can hold index n */
    if (cache_len <= n) {
        idx_t new_len = (cache_len == 0) ? (n + 1) : (cache_len * 2);
        if (new_len <= n)
            new_len = n + 1;

        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        if (!new_cache)
            abort();  /* allocation failure */

        /* Copy existing values, if any */
        if (cache != NULL)
            memcpy(new_cache, cache, sizeof(seq_t) * cache_len);

        /* Initialise newly allocated slots (optional, 0 is safe) */
        if (new_cache + cache_len < new_cache + new_len)
            memset(new_cache + cache_len, 0,
                   sizeof(seq_t) * (new_len - cache_len));

        /* Replace old cache */
        if (cache != NULL)
            free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    /* Compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 4) {
            cache[i] = 0;
        } else if (i == 5) {
            cache[i] = 1;
        } else {
            cache[i] = 15 * cache[i - 1]
                     - 86 * cache[i - 2]
                     + 234 * cache[i - 3]
                     - 300 * cache[i - 4]
                     + 144 * cache[i - 5];
        }
    }

    last_computed = (last_computed < n) ? n : last_computed;
    return cache[n];
}
