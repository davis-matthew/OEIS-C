/*
 * A039057  Numbers whose base-8 representation has the same number of 1's and
 *          3's.
 * https://oeis.org/A039057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers whose base-8 representation has equal number of
 *            1's and 3's
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff x has the same number of 1’s and 3’s in base‑8. */
static bool is_balanced_base8(seq_t x)
{
    int cnt1 = 0, cnt3 = 0;
    if (x == 0) {
        return true; /* 0 has 0 ones and 0 threes. */
    }
    while (x != 0) {
        int digit = (int)(x % 8);
        if (digit == 1)
            ++cnt1;
        else if (digit == 3)
            ++cnt3;
        x /= 8;
    }
    return cnt1 == cnt3;
}

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_count = 0;
    static seq_t last_checked = 0;   /* next candidate to test */

    if (n == 0) {
        return (seq_t)0; /* undefined for offset 1, but safe to return 0 */
    }

    while (cache_count < n) {
        if (is_balanced_base8(last_checked)) {
            /* Ensure cache has space. */
            if (cache_count == cache_cap) {
                idx_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_count++] = last_checked;
        }
        last_checked++;  /* move to next integer */
    }

    return cache[n - 1];
}
