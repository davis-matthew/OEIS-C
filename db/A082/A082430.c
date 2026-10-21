/*
 * A082430  a(1)=1; for n > 1, a(n) = n*(a(n-1) + a(n-2) + ... + a(2) + a(1))
 *          + 4.
 * https://oeis.org/A082430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(a(n-1)+a(n-2)+...+a(1)) + 4 with a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Cached values – index 0 is unused */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* highest index computed so far     */
    static seq_t sum_cache = 0;   /* Σ_{i=1}^{cap} a(i)                */

    if (n == 0) return 0;        /* safety – real sequence starts at 1 */

    /* Extend the cache if necessary */
    if ((size_t)n > cap) {
        size_t new_len = (size_t)n + 1;   /* array length = max index + 1 */

        if (cache == NULL) {           /* first allocation         */
            cache = seq_alloc(new_len);
            if (!cache) exit(EXIT_FAILURE);
        } else {                       /* grow the existing array  */
            seq_t *tmp = seq_realloc(cache, cap + 1, new_len);
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
        }

        /* Compute new terms */
        idx_t i_start = (idx_t)(cap + 1);
        if (i_start <= 1) {           /* initialise a(1)           */
            cache[1] = (seq_t)1;
            sum_cache = (seq_t)1;
            i_start = 2;
            cap = 1;
        }

        for (idx_t i = i_start; i <= n; ++i) {
            seq_t val = (seq_t)i * sum_cache + (seq_t)4;
            cache[i] = val;
            sum_cache += val;
        }

        cap = (size_t)n;
    }

    return cache[n];
}
