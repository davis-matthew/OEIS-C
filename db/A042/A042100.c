/*
 * A042100  Numerators of continued fraction convergents to sqrt(574).
 * https://oeis.org/A042100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 1150*a(n-4) - a(n-8) for n >= 8 Initial values: a(0)=23,
 *            a(1)=24, a(2)=551, a(3)=575, a(4)=27001, a(5)=27576,
 *            a(6)=633673, a(7)=661249
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 198 (2026-11-03)
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_cap)
        return;
    size_t old_cap = cache_cap;
    grow_seq(&cache, &cache_cap, need);
    for (size_t i = old_cap; i < cache_cap; ++i) {
        if (i < 8) {
            switch (i) {
                case 0: cache[i] = 23; break;
                case 1: cache[i] = 24; break;
                case 2: cache[i] = 551; break;
                case 3: cache[i] = 575; break;
                case 4: cache[i] = 27001; break;
                case 5: cache[i] = 27576; break;
                case 6: cache[i] = 633673; break;
                case 7: cache[i] = 661249; break;
            }
        } else {
            cache[i] = 1150 * cache[i-4] - cache[i-8];
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
