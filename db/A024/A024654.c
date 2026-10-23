/*
 * A024654  n written in fractional base 9/6.
 * https://oeis.org/A024654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the representation of n in fractional base 9/6
 *            interpreted as a decimal integer (digits 0‑8)
 * Range:     u8 n <= 17, u16 n <= 71, u32 n <= 368, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static seq_t *cache = NULL;            /* cache of a(0..max)  */
static size_t cache_cap = 0;           /* capacity of cache   */
static size_t cache_len = 0;           /* number of values stored */

static uint8_t *digit_arr = NULL;      /* digits of the current number (least significant first) */
static size_t digit_cap = 0;           /* capacity of digit_arr   */
static size_t digit_len = 0;           /* number of digits used   */

#ifdef __cplusplus
/* Grow the cache array using C++ new[] (constructors called) */
static void grow_cache(size_t needed) {
    if (needed <= cache_cap) return;
    seq_t *new_cache = new seq_t[needed];
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        delete[] cache;
    }
    cache = new_cache;
    cache_cap = needed;
}
#else
/* Grow the cache array using realloc (works for POD types) */
static void grow_cache(size_t needed) {
    if (needed <= cache_cap) return;
    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache && needed > 0) abort();
    cache = new_cache;
    cache_cap = needed;
}
#endif

static void grow_digits(size_t needed) {
    if (needed <= digit_cap) return;
    size_t new_cap = digit_cap ? digit_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(digit_arr, new_cap);
    if (!tmp && needed > 0) abort();
    digit_arr = tmp;
    digit_cap = new_cap;
}

static void ensure_initialized(void) {
    if (cache_len) return;          /* already initialized */
    /* cache for a(0) */
    grow_cache(1);
    cache[0] = 0;
    cache_len = 1;
    /* digits for 0: single digit 0 */
    grow_digits(1);
    digit_arr[0] = 0;
    digit_len = 1;
}

seq_t a(idx_t n) {
    ensure_initialized();

    /* compute all values up to n if needed */
    if ((idx_t)cache_len <= n) {
        grow_cache((size_t)n + 1);   /* ensure capacity for index n */

        /* incrementally generate values from current max to n */
        while ((idx_t)cache_len <= n) {
            /* add 1 to the representation in fractional base 9/6 */
            digit_arr[0] += 1;
            size_t pos = 0;
            while (digit_arr[pos] >= 9) {          /* digit overflow? */
                digit_arr[pos] -= 9;               /* subtract a=9 */
                if (pos + 1 == digit_len) {        /* extend array if needed */
                    grow_digits(digit_len + 1);
                    digit_arr[digit_len] = 0;
                    digit_len++;
                }
                digit_arr[pos + 1] += 6;           /* carry b=6 to next digit */
                pos++;
            }

            /* convert the digit array to a decimal integer */
            seq_t val = 0;
            for (size_t k = digit_len; k-- > 0;) {
                val = val * 10 + (seq_t)digit_arr[k];
            }
            cache[cache_len++] = val;
        }
    }
    return cache[n];
}
