/*
 * A039582  Numbers whose base-6 representation has the same number of 0's,
 *          4's and 5's.
 * https://oeis.org/A039582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th positive integer whose base‑6 representation
 *            contains an equal number of 0’s, 4’s, and 5’s
 * Range:     u8 n <= 45, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper that tests whether a given integer satisfies the condition. */
static bool good(seq_t x)
{
    int c0 = 0, c4 = 0, c5 = 0;
    if (x == 0)            /* base‑6 of 0 has no 0,4,5 – counts are 0,0,0 */
        return true;
    seq_t t = x;
    while (t != 0) {
        int d = (int)(t % 6);   /* remainder < 6, safe to cast to int */
        if (d == 0) c0++;
        else if (d == 4) c4++;
        else if (d == 5) c5++;
        t = t / 6;
    }
    return (c0 == c4) && (c4 == c5);
}

/* Cache of found terms and state of the sieve. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t last_scanned = 0;   /* last number examined in the search */

/* Ensure the cache has capacity for at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_ptr == NULL) {
        /* In a real program we'd handle allocation failure; here we just abort. */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;       /* undefined for offset 1, but harmless */

    if (n <= cache_len)
        return cache[n - 1];

    /* Extend the search until we have at least 'n' suitable numbers. */
    ensure_cache_cap(n);
    while (cache_len < (size_t)n) {
        ++last_scanned;                 /* examine next integer */
        if (good(last_scanned)) {
            cache[cache_len++] = last_scanned;
        }
    }
    return cache[n - 1];
}
