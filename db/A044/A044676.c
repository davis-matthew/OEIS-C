/*
 * A044676  Numbers n such that string 4,8 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure: a(n) is the n‑th integer whose base‑9 representation
 * contains the substring "48" but the next integer does not. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: test whether x in base 9 contains the digit pair 4,8 */
static bool has48_base9(seq_t x)
{
    if (x == 0) return false;
    uint8_t digits[64];       /* sufficient for any value handled */
    int len = 0;
    while (x > 0) {
        int d = (int)(x % (seq_t)9);
        digits[len++] = (uint8_t)d;
        x /= (seq_t)9;
    }
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] == 4 && digits[i - 1] == 8)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_checked = 0;

    if (n == 0) return 0;   /* undefined for n < 1 */

    /* grow cache until we have at least n terms */
    while (cache_len < n) {
        last_checked++;
        if (has48_base9(last_checked) && !has48_base9(last_checked + 1)) {
            /* ensure capacity */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();   /* insufficient memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;
        }
    }
    return cache[n - 1];
}
