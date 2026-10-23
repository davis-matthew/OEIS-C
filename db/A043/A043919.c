/*
 * A043919  Numbers k such that 1 and 4 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th integer k whose base‑7 representation
   contains the adjacent digits 1 and 4 in either order, and whose
   successor k+1 does not. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper: does x in base 7 contain the adjacent pattern 1‑4 or 4‑1 ? */
static bool has14(seq_t x)
{
    if (x == (seq_t)0) return false;
    uint8_t digits[32];
    size_t len = 0;
    while (x > (seq_t)0) {
        digits[len++] = (uint8_t)(x % (seq_t)7);
        x /= (seq_t)7;
    }
    for (size_t i = 0; i + 1 < len; ++i) {
        if ((digits[i] == 1 && digits[i + 1] == 4) ||
            (digits[i] == 4 && digits[i + 1] == 1))
            return true;
    }
    return false;
}

/* Static storage for memoization */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static seq_t last_checked = (seq_t)0;

/* Ensure cache can hold at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main function exposed to the framework */
seq_t a(idx_t n)
{
    /* n >= 1 (OEIS offset) is guaranteed by framework */
    while (cache_size < (size_t)n) {
        seq_t cand = last_checked + (seq_t)1;
        if (has14(cand) && !has14(cand + (seq_t)1)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = cand;
        }
        last_checked = cand;
    }
    return cache[n - 1];
}
