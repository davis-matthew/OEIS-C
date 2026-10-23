/*
 * A126413  Number of base 26 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   grow_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A126413:
   a(0) = 1;
   For n ≥ 1 let f(1,d) = 1 for all d in [1,26].
   Then f(k + 1,d) = Σ_{t:|t – d|≤2} f(k,t).
   a(n) = Σ_{d=1}^{26} f(n,d). */
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;          /* a[0 … max_n]            */
static size_t cache_cap = 0;         /* size of allocated cache */
static idx_t cache_len = 0;          /* highest n computed      */

static seq_t *f_curr = NULL;         /* f(n,1…26) for current n*/
static seq_t *next_vec = NULL;       /* temporary DP buffer      */

/* Compute a(n) for base 26 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* first-time initialization */
    if (cache == NULL) {
        /* cache at least for a(0) and a(1) */
        cache_cap = 2;
        cache = seq_alloc_array(cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = 0;               /* placeholder */

        /* DP arrays */
        f_curr   = seq_alloc_array(26);
        next_vec = seq_alloc_array(26);
        for (idx_t i = 0; i < 26; ++i)
            f_curr[i] = (seq_t)1;

        /* a(1) */
        seq_t sum = 0;
        for (idx_t i = 0; i < 26; ++i)
            sum += f_curr[i];
        cache[1] = sum;
        cache_len = 1;
    }

    /* grow cache if necessary */
    if (n + 1 > cache_cap)
        grow_seq_array(&cache, &cache_cap, n + 1);

    if (n <= cache_len)
        return cache[n];

    /* compute a(cache_len+1) … a(n) */
    for (idx_t step = cache_len + 1; step <= n; ++step) {
        /* zero next buffer */
        for (idx_t i = 0; i < 26; ++i)
            next_vec[i] = 0;

        /* transition */
        for (idx_t d = 1; d <= 26; ++d) {
            idx_t start = (d > 2) ? d - 2 : 1;
            idx_t end   = (d + 2 <= 26) ? d + 2 : 26;
            seq_t s = 0;
            for (idx_t k = start; k <= end; ++k)
                s += f_curr[k - 1];
            next_vec[d - 1] = s;
        }

        /* sum for a(step) */
        seq_t sum = 0;
        for (idx_t i = 0; i < 26; ++i)
            sum += next_vec[i];
        cache[step] = sum;

        /* swap buffers for next iteration */
        seq_t *tmp = f_curr;
        f_curr = next_vec;
        next_vec = tmp;
    }

    cache_len = n;
    return cache[n];
}
