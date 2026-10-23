/*
 * A121026  Multiples of 6 containing a 6 in their decimal representation.
 * https://oeis.org/A121026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: nth multiple of 6 that contains digit 6 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper: return true if x (in decimal) contains the digit 6 */
static bool contains_digit6(seq_t x)
{
    while (x > 0) {
        if (x % 10 == 6) return true;
        x /= 10;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n == 0 */

    static seq_t *cache = NULL;
    static size_t cached_size = 0;
    static size_t cache_cap = 0;
    static seq_t cur = 6;       /* current candidate multiple of 6 */
    static bool initialized = false;

    /* One‑time initialization */
    if (!initialized) {
        cache_cap = 128;                  /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failed */
        cached_size = 0;
        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (cached_size < (size_t)n) {
        if (contains_digit6(cur)) {
            if (cached_size >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;       /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cached_size++] = cur;
        }
        cur += 6;                         /* next multiple of 6 */
    }

    return cache[(size_t)n - 1];
}
