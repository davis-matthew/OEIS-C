/*
 * A118153  Start with 1 and repeatedly reverse the digits and add 57 to get
 *          the next term.
 * https://oeis.org/A118153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=rev(a(n-1))+57 for n>1, where rev(x) reverses the decimal digits of x */
#include <stdlib.h>

static seq_t reverse_digits(seq_t x) {
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + x % 10;
        x /= 10;
    }
    return rev;
}

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;                     /* offset starts at 1, behaviour for 0 is irrelevant */
    }
    /* Cache index 0 corresponds to a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;        /* number of terms stored */
    static idx_t cache_cap = 0;        /* allocated capacity */

    if (cache_len == 0) {              /* initialisation */
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) {                  /* allocation failed, return 0 */
            return 0;
        }
        cache[0] = 1;                  /* a(1) = 1 */
        cache_len = 1;
    }

    if (n > cache_len) {
        /* Extend cache to hold terms up to index n */
        idx_t new_cap = cache_cap;
        while (new_cap < n) {
            new_cap <<= 1;              /* grow by factor 2 */
        }
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) {              /* realloc failed, return 0 */
            free(cache);
            cache = NULL;
            cache_len = 0;
            cache_cap = 0;
            return 0;
        }
        cache = new_cache;
        cache_cap = new_cap;

        for (idx_t i = cache_len; i < n; ++i) {
            cache[i] = reverse_digits(cache[i - 1]) + 57;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
