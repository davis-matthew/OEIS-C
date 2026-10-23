/*
 * A045300  Numbers whose base-5 representation contains exactly two 3's and
 *          no 4's.
 * https://oeis.org/A045300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by linear scan for numbers whose base‑5 representation
   contains exactly two 3's and no 4's.  Terms are cached so
   subsequent calls are O(1). */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements cached */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t start = (cache_len == 0) ? 0 : cache[cache_len - 1] + 1;
        seq_t candidate = start;
        int found = 0;           /* 1 if candidate is valid */

        /* Find the next valid number */
        while (!found) {
            seq_t x = candidate;
            int cnt3 = 0;
            int ok = 1;

            if (x == 0) {
                ok = 0;           /* zero has no digits */
            } else {
                while (x > 0) {
                    int d = (int)(x % 5);
                    if (d == 4) { ok = 0; break; }
                    if (d == 3) {
                        ++cnt3;
                        if (cnt3 > 2) { ok = 0; break; }
                    }
                    x /= 5;
                }
                if (ok && cnt3 == 2) found = 1;
            }

            if (!found) ++candidate;
        }

        /* Store the found term */
        if (cache_len == cache_cap) {
            size_t newcap = (cache_cap == 0) ? 4 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cache_cap = newcap;
        }
        cache[cache_len++] = candidate;
    }

    return cache[n - 1];
}
