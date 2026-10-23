/*
 * A043788  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 3 mod 6.
 * https://oeis.org/A043788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the number of runs in the base‑3
 *            representation of n is ≡ 3 (mod 6)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached terms */
static size_t cache_cap = 0;     /* allocated size */
static idx_t cache_len = 0;      /* number of terms stored */
static uint64_t current = 0;     /* next integer to test */

/* Count runs in the base‑3 representation of x. */
static int runs_in_base3(uint64_t x)
{
    if (x == 0) return 1;        /* base‑3 of 0 is “0” – one run */
    int runs = 0;
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 3);
        if (d != prev) {
            ++runs;
            prev = d;
        }
        x /= 3;
    }
    return runs;
}

/* Grow the cache array to at least new_cap elements. */
static void ensure_cache_cap(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap * 2 : 16;
    while (cap < new_cap) cap *= 2;
    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    cache_cap = cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n<1, return 0 */

    /* Ensure the cache holds at least n terms. */
    if (cache_len < n) {
        ensure_cache_cap((size_t)n);
        while (cache_len < n) {
            while (1) {
                uint64_t val = current++;
                if (runs_in_base3(val) % 6 == 3) {
                    cache[cache_len++] = (seq_t)val;
                    break;
                }
            }
        }
    }
    return cache[n - 1];
}
