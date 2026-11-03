/*
 * A017843  Expansion of 1/(1-x^5-x^6-x^7-x^8-x^9-x^10-x^11-x^12).
 * https://oeis.org/A017843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ordered compositions of n into parts 5..12
 *            Recurrence: a(0) = 1; a(n)=0 for 1≤n<5; a(n) =
 *            a(n-5)+a(n-6)+…+a(n-12) for n≥5
 * Range:     u8 n <= 28, u16 n <= 49, u32 n <= 90, u64 n <= 174,
 *            u128 n <= 341, big any n
 * Validated: standard, pass, 63 terms through n = 985 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* capacity of cache */
    static size_t used = 0;  /* number of values computed (highest index+1) */

    /* initial allocation and seed base cases */
    if (cap == 0) {
        grow_seq(&cache, &cap, 13);     /* indices 0..12 */
        for (idx_t i = 1; i < 5; ++i)
            cache[i] = 0;
        cache[0] = 1;
        /* compute for 5..12 using recurrence */
        for (idx_t i = 5; i < 13; ++i) {
            seq_t sum = 0;
            for (int j = 5; j <= 12; ++j) {
                if (i >= j)
                    sum += cache[i - j];
            }
            cache[i] = sum;
        }
        used = 13;
    }

    /* enlarge cache if needed and compute missing terms */
    if ((idx_t)cap <= n) {
        grow_seq(&cache, &cap, (size_t)n + 1);
    }
    if (used <= n) {
        for (idx_t i = used; i <= n; ++i) {
            if (i < 5) {
                cache[i] = 0;
            } else {
                seq_t sum = 0;
                for (int j = 5; j <= 12; ++j) {
                    if (i >= (idx_t)j)
                        sum += cache[i - j];
                }
                cache[i] = sum;
            }
        }
        used = (size_t)n + 1;
    }

    return cache[n];
}
