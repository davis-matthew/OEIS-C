/*
 * A043139  Numbers k such that 1 and 4 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Computes the sequence of integers k such that the base‑7 representation
   of k contains adjacent digits 1 and 4 (in either order), while the
   base‑7 representation of k‑1 does not.  The function is memoised so
   calls can be in any order. */
static int has14(seq_t x)
{
    if (x == 0)
        return 0;
    uint8_t digits[64];
    size_t len = 0;
    while (x > 0) {
        digits[len++] = (uint8_t)(x % 7);
        x /= 7;
    }
    if (len < 2)
        return 0;
    for (size_t i = 1; i < len; ++i) {
        if ((digits[i] == 1 && digits[i-1] == 4) ||
            (digits[i] == 4 && digits[i-1] == 1))
            return 1;
    }
    return 0;
}

static seq_t *cache = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;
static seq_t    next_k    = 1;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset starts at 1 */

    if ((idx_t)cache_len >= n)
        return cache[n-1];

    while ((idx_t)cache_len < n) {
        if (has14(next_k) && !has14(next_k - 1)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *np = mem_resize(seq_t, cache, new_cap);
                if (!np) abort();
                cache = np;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_k;
        }
        ++next_k;
    }

    return cache[n-1];
}
