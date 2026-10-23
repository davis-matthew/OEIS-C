/*
 * A119384  Ten's complement of the factorials.
 * https://oeis.org/A119384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 10^digits(n!) - n!
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   digit_len, pow10, seq_alloc, seq_one, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_one.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* 0-indexed factorial cache, cache[0] = 1! = 1 */
    static seq_t *fact_cache = NULL;
    static idx_t  max_cached = 0;   /* highest n for which factorial is cached */
    static size_t cache_cap  = 0;   /* array capacity */

    if (n == 0) return 0; /* not required, but safe for out‑of‑range */

    /* Grow the cache if necessary */
    if (n > max_cached) {
        size_t new_cap = (size_t)n + 1;
        if (fact_cache == NULL) {
            fact_cache = seq_alloc(new_cap);
            cache_cap  = new_cap;
            fact_cache[0] = seq_one();
            max_cached   = 0;
        } else if (new_cap > cache_cap) {
            fact_cache = seq_realloc(fact_cache, cache_cap, new_cap);
            cache_cap  = new_cap;
        }

        /* Compute factorials from max_cached+1 up to n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            fact_cache[i] = fact_cache[i-1] * (seq_t)i;
        }
        max_cached = n;
    }

    seq_t f = fact_cache[n];
    int    d = digit_len(f);           /* number of decimal digits of n! */
    seq_t  p10 = pow10((idx_t)d);      /* 10^d */
    return p10 - f;                    /* ten's complement of n! */
}
