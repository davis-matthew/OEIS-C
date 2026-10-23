/*
 * A039184  Numbers whose base-11 representation has the same number of 3's
 *          and 6's.
 * https://oeis.org/A039184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   has_equal_36
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute‑force search of natural numbers and base‑11 digit counting */
#include <stdlib.h>
#include "../utils/has_equal_36.h"

static seq_t *cache = NULL;      /* stored valid terms */
static size_t cache_cap = 0;     /* current capacity of cache */
static size_t cache_len = 0;     /* number of terms cached */
static seq_t next_val = 0;       /* next integer to test */

/* grow the cache until it holds at least n terms */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        if (has_equal_36(next_val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_val;
        }
        next_val = next_val + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0, return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
