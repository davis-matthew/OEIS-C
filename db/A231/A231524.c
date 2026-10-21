/*
 * A231524  Number of 2 X n 0..1 arrays with no element less than a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A231524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + a(n-3) + 6*a(n-4) - 18*a(n-5), with
 *            initial terms a(1)=4, a(2)=10, a(3)=34, a(4)=107, a(5)=342
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;          /* allocated size (index + 1) */
static idx_t max_computed = 0;       /* highest n whose value is filled */

static const seq_t base_vals[6] = { 0, 4, 10, 34, 107, 342 };

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                   /* out‑of‑range indices are undefined */

    /* Allocate or grow the cache as needed */
    if (cache_len < n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache,
                                       (n + 1) * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            cache_len = 0;
            return 0;               /* allocation failed */
        }
        cache     = tmp;
        cache_len = n + 1;

        /* Initialise known base values that may not yet exist */
        for (idx_t i = 1; i <= 5 && i < cache_len; ++i)
            cache[i] = base_vals[i];
    }

    /* If we already have the desired value: return it */
    if (max_computed >= n)
        return cache[n];

    /* Compute missing terms, ensuring that indices < 6 use fixed constants */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 5) {
            cache[i] = base_vals[i];
        } else {
            cache[i] = ((seq_t)4   * cache[i-1])
                     - ((seq_t)3   * cache[i-2])
                     +          cache[i-3]
                     + ((seq_t)6   * cache[i-4])
                     - ((seq_t)18  * cache[i-5]);
        }
    }

    max_computed = n;
    return cache[n];
}
