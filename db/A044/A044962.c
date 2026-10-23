/*
 * A044962  Numbers with no two equally numerous base 13 digits.
 * https://oeis.org/A044962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate integers and keep those whose non‑zero digit counts
 * in base 13 are pairwise distinct. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1, unused case */

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* Ensure the cache can hold n elements. */
    if (cap == 0) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    if (n > cap) {
        cap = n * 2;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* If we already have a(n), return it. */
    if (len >= n) {
        return cache[n - 1];
    }

    /* Start searching from the first positive integer not yet examined. */
    seq_t cand = (len == 0) ? 1 : cache[len - 1] + 1;

    while (1) {
        /* Count digits in base 13. */
        int freq[13] = {0};
        seq_t m = cand;
        while (m > 0) {
            int d = (int)(m % 13);
            freq[d]++;
            m /= 13;
        }

        /* Test that all positive counts are distinct. */
        bool used[128] = {false};
        bool ok = true;
        for (int i = 0; i < 13 && ok; ++i) {
            if (freq[i] > 0) {
                int f = freq[i];
                if (used[f]) {
                    ok = false;
                } else {
                    used[f] = true;
                }
            }
        }

        if (ok) {
            /* Store the candidate. */
            cache[len++] = cand;
            if (len == n) {
                return cand;
            }
        }

        cand++;
    }
}
