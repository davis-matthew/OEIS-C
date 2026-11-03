/*
 * A044274  Numbers k such that the string 2,5 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9534 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers k such that:
 *   - the base‑9 representation of k contains the substring "25"
 *   - the base‑9 representation of k‑1 does NOT contain "25"
 * a(1) is the first such k (23).  The function accepts any
 * index n >= 1 and may be called in arbitrary order.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: does the base‑9 representation of x contain "25"? */
static bool has25_base9(seq_t x) {
    /* 9‑digit array; sufficient for seq_t up to 128 bits (max 41 digits) */
    unsigned char digits[64];
    size_t len = 0;
    seq_t tmp = x;

    while (tmp > 0) {
        seq_t rem = tmp % 9;
        digits[len++] = (unsigned char)rem;   /* 0 <= rem <= 8 */
        tmp /= 9;
    }
    if (len < 2) return false;

    /* scan forward from most significant digit */
    for (size_t i = len; i > 1; --i) {
        if (digits[i-1] == 2 && digits[i-2] == 5) return true;
    }
    return false;
}

/* Main sequence accessor */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined for n<1, return 0 */

    /* static state for memoization */
    static seq_t *cache = NULL;         /* array of found terms */
    static size_t cache_cap = 0;        /* allocated capacity */
    static size_t cache_len = 0;        /* number of terms stored */
    static seq_t next_to_test = 1;      /* next integer to examine */

    /* extend cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t curr = next_to_test++;

        if (has25_base9(curr) && !has25_base9(curr - 1)) {
            /* need space in cache */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                if (!new_cache) return 0;   /* allocation failure – return 0 */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = curr;
        }
    }
    return cache[n - 1];
}
