/*
 * A033064  Numbers whose base-13 representation Sum_{i=0..m} d(i)*13^i has
 *          odd d(m-i) for all odd i.
 * https://oeis.org/A033064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer whose base‑13 representation *
 *            satisfies that for every odd index i, the digit d(m−i) (where
 *            d(j) * denotes the j‑th least significant base‑13 digit) is odd.
 *            * The function is computed by successive search and memoisation
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether a number satisfies the digit‑parity condition. */
static int has_property(seq_t v)
{
    seq_t y = v;
    uint8_t digits[64];
    size_t len = 0;

    /* decompose the number into base‑13 digits (least significant first) */
    while (y) {
        digits[len++] = (uint8_t)(y % 13);
        y = y / 13;
    }
    if (len == 0)              /* 0 is not part of the sequence */
        return 0;

    size_t m = len - 1;        /* index of the most significant digit */
    size_t j;
    for (j = 0; j < len - 1; ++j) {
        /* condition: if parity(m) != parity(j) then d[j] must be odd */
        if ( ((m & 1) && !(j & 1)) || (!(m & 1) && (j & 1)) ) {
            if ((digits[j] & 1) == 0)    /* even digit */
                return 0;
        }
    }
    return 1;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* cached sequence values */
    static seq_t *cache = NULL;     /* array of seq_t values */
    static size_t cache_cap = 0;    /* current allocated capacity */
    static idx_t  cache_len = 0;    /* number of values stored (offset 1) */

    if (n == 0)                    /* safety: 0 is undefined in the OEIS offset */
        return 0;

    /* Return cached value if already computed */
    if (cache_len >= n)
        return cache[n - 1];

    /* Ensure capacity for at least n elements */
    if (cache_len == 0) {
        cache_cap = 32;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Start searching from the smallest integer following the last cached one */
    seq_t current = (cache_len == 0) ? 1 : (cache[cache_len - 1] + 1);

    while (cache_len < n) {
        if (has_property(current)) {
            /* enlarge cache if needed */
            if ((size_t)cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
                if (!tmp) {           /* allocation failed, return 0 as safe guard */
                    free(cache);
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = current;
        }
        ++current;
    }

    return cache[n - 1];
}
