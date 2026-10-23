/*
 * A118530  Start with 1 and repeatedly reverse the digits and add 13 to get
 *          the next term.
 * https://oeis.org/A118530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = rev(a(n-1)) + 13, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* rev(x) reverses the decimal digits of x. */

#include <stdlib.h>

/* Reverse the decimal digits of an unsigned seq_t value.  */
static seq_t reverse_seq_t(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        seq_t digit = x % 10;
        rev = rev * 10 + digit;
        x = x / 10;
    }
    return rev;
}

/* Memoised computation of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset behaviour is irrelevant below 1 */

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Initialise cache with the first term a(1) = 1. */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;        /* allocation failure – unlikely in tests */
        cache[0] = 1;
        cache_len = 1;
    }

    /* Extend the cache up to the requested index. */
    if (n > cache_len) {
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t rev  = reverse_seq_t(prev);
            seq_t next = rev + (seq_t)13;

            /* Grow the cache if needed. */
            if (i >= cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
                while (new_cap <= i) new_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
                cache_cap = new_cap;
            }
            cache[i] = next;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
