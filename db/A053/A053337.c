/*
 * A053337  a(n) contains n digits (either '6' or '7') and is divisible by
 *          2^n.
 * https://oeis.org/A053337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 10^(n-1)*(6+[a(n-1)/2^(n-1) mod 2]), a(1) = 6
 *            (recurrence form)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached terms */
static idx_t cache_len = 0;          /* number of cached terms */
static idx_t cache_cap = 0;          /* current capacity of cache */
static seq_t pow10 = 0;              /* 10^(current_index-1), ready for next term */

/* Ensure cache has space for at least new_cap terms */
static void resize_cache(idx_t new_cap) {
    if (new_cap <= cache_cap)
        return;
    seq_t *newp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
}

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* below offset, no defined behaviour */

    /* Handle first term */
    if (cache_len == 0) {
        resize_cache(1);
        cache[0] = 6;
        cache_len = 1;
        pow10 = 10;        /* ready for next term (n=2) */
    }

    /* Expand cache up to n if needed */
    if (n > cache_len) {
        resize_cache((size_t)n);
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            /* bit = (a(k-1) / 2^(k-1)) mod 2 */
            seq_t bit = (cache[cache_len - 1] >> (k - 1)) & (seq_t)1;
            /* digit = 6 + bit  (6 or 7) */
            seq_t digit = (seq_t)6 + bit;
            /* a(k) = a(k-1) + pow10 * digit */
            seq_t ak = cache[cache_len - 1] + pow10 * digit;
            cache[cache_len] = ak;
            ++cache_len;
            /* prepare pow10 for next iteration: 10^(k) */
            pow10 *= (seq_t)10;
        }
    }

    return cache[n - 1];
}
