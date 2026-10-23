/*
 * A039104  Numbers whose base-9 representation has the same number of 4's and
 *          7's.
 * https://oeis.org/A039104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth nonnegative integer whose base-9 representation
 *            contains equal numbers of 4 and 7. Computed by brute-force
 *            search
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool is_special(seq_t x) {
    int cnt4 = 0, cnt7 = 0;
    if (x == 0) return true; /* 0 has equal 4's and 7's (both 0) */
    while (x > 0) {
        seq_t d = x % 9;
        if (d == 4) cnt4++;
        else if (d == 7) cnt7++;
        x /= 9;
    }
    return cnt4 == cnt7;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset 1 – but n>=1 in tests */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 0; /* start from 0 */

    /* Ensure enough space in cache */
    while (cache_len < (size_t)n) {
        /* Find next qualifying number */
        while (!is_special(next_candidate)) next_candidate++;

        /* Grow cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 64;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len++] = next_candidate;
        next_candidate++; /* next search starts after stored number */
    }

    return cache[n - 1];
}
