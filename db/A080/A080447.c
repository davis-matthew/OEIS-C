/*
 * A080447  a(1) = 3, a(n) = smallest (nontrivial) multiple of a(n-1)
 *          containing n digits, a(n) not equal to 10*a(n-1).
 * https://oeis.org/A080447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest non‑trivial multiple of a(n-1) that has n digits
 *            and is not equal to 10*a(n-1); a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */
#include "../utils/pow10.h"         /* seq_t pow10(idx_t e) */

seq_t a(idx_t n)
{
    /* static cache and bookkeeping */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  last_computed = 0;   /* highest index that has been computed */

    if (n == 0) return (seq_t)0; /* undefined, but safe for sub‑calls */

    /* make array large enough */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* base term */
    if (n == 1) {
        if (last_computed < 1) {
            cache[1] = (seq_t)3;
            last_computed = 1;
        }
        return cache[1];
    }

    /* compute terms up to n iteratively to keep stack usage small */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 1) {                 /* already handled above */
            cache[1] = (seq_t)3;
            continue;
        }

        seq_t prev = cache[i-1];

        /* compute k_min = ceil(10^(i-1) / prev) */
        seq_t pow_i_minus1 = pow10(i-1);
        seq_t k_min = (pow_i_minus1 + prev - (seq_t)1) / prev;

        /* skip the trivial multiplier 10 */
        if (k_min == (seq_t)10) k_min += (seq_t)1;

        /* result = k_min * prev */
        cache[i] = k_min * prev;
    }

    last_computed = n;
    return cache[n];
}
