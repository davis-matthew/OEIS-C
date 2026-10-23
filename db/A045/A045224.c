/*
 * A045224  Numbers whose base-5 representation contains exactly one 1 and
 *          three 2's.
 * https://oeis.org/A045224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045224: Numbers whose base‑5 representation contains exactly
 * one digit '1' and exactly three digits '2'.  The function searches
 * integers in increasing order and returns the nth such integer
 * (n >= 1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Test whether an integer has exactly one '1' and three '2's in base‑5 */
static bool matches(idx_t x)
{
    int cnt1 = 0, cnt2 = 0;
    while (x > 0) {
        int d = x % 5;
        if (d == 1) {
            if (++cnt1 > 1) return false;
        } else if (d == 2) {
            if (++cnt2 > 3) return false;
        }
        x /= 5;
    }
    return cnt1 == 1 && cnt2 == 3;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1 */

    static seq_t *cache   = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 0;

    /* Extend cache until it contains at least n terms */
    while (cache_sz < n) {
        last_checked++;

        if (matches(last_checked)) {
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz] = (seq_t)last_checked;
            cache_sz++;
        }
    }

    return cache[n - 1];
}
