/*
 * A127650  a(n) = n^n - (2n-1)!!.
 * https://oeis.org/A127650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - (2n-1)!!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* Return 0 for n==0 (outside the OEIS offset). */
    if (n == 0)
        return 0;

    /* Static cache for double factorial (2k-1)!! */
    static seq_t *df_cache = NULL;
    static size_t df_cap   = 0;
    static idx_t  df_max   = 0;   /* highest n for which df_cache[n] is set */

    /* Ensure cache array has room for indices up to n. */
    ensure_cache(n, &df_cache, &df_cap);

    /* Compute double factorial up to n if needed. */
    if (n > df_max) {
        for (idx_t k = df_max + 1; k <= n; ++k) {
            if (k == 1) {
                df_cache[1] = 1;
            } else {
                df_cache[k] = df_cache[k-1] * ( (seq_t)(2*k - 1) );
            }
        }
        df_max = n;
    }

    /* Compute n^n using the special integer‑pow utility. */
    seq_t pow_val = ipow_idx((seq_t)n, n);

    /* Subtract the double factorial. */
    return pow_val - df_cache[n];
}
