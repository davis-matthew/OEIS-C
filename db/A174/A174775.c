/*
 * A174775  y-values in the solution to x^2 - 37*y^2 = 1.
 * https://oeis.org/A174775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 146*a(n-1)-a(n-2) with a(1)=0, a(2)=12
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 186 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

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
static idx_t cached_max = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but safe */
    if (!cache) {                  /* first time: initialise cache */
        cache_cap = 3;             /* reserve indices 0,1,2 */
        cache = alloc_seq(cache_cap);
        cache[0] = 0;              /* unused slot */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)12;
        cached_max = 2;
    }
    if (n <= cached_max) {
        return cache[n];
    }
    if ((size_t)(n + 1) > cache_cap) {
        grow_seq(&cache, &cache_cap, (size_t)(n + 1));
    }
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        cache[i] = ((seq_t)146) * cache[i - 1] - cache[i - 2];
    }
    cached_max = n;
    return cache[n];
}
