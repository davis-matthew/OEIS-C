/*
 * A039331  Numbers whose base-8 representation has the same nonzero number of
 *          0's and 3's.
 * https://oeis.org/A039331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑8 representation contains the same
   non‑zero number of 0's and 3's.  a(n) is the n-th such positive integer
   with the offset 1.  The implementation keeps a static cache so that
   the function can be called in any order and is efficient for n up
   to about 10 000. */
#include <stdlib.h>
#include <stdbool.h>

/* Return true if the base‑8 digits of x contain equal positive numbers
   of 0 and 3. */
static bool has_equal_zeros_and_threes(seq_t x)
{
    unsigned char cnt0 = 0, cnt3 = 0;
    while (x > 0) {
        seq_t d = x % 8;
        if (d == 0)      cnt0++;
        else if (d == 3) cnt3++;
        x /= 8;
    }
    return (cnt0 == cnt3) && (cnt0 > 0);
}

/* Cache of found terms */
static seq_t *cache      = NULL;   /* array of saved terms      */
static idx_t  cache_len  = 0;      /* number stored so far      */
static idx_t  cache_cap  = 0;      /* current allocated size    */
static seq_t  next_candidate = 1;  /* next integer to test      */

/* Extend the cache until it contains at least n_needed elements. */
static void build_cache(idx_t n_needed)
{
    if (cache_len >= n_needed) return;

    if (!cache) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_len < n_needed) {
        if (has_equal_zeros_and_threes(next_candidate)) {
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }
}

/* External function required by the build.  n >= 1.  For n == 0 it
   returns 0 (undefined offset). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, but safe return */
    build_cache(n);
    return cache[n - 1];
}
