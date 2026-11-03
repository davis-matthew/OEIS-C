/*
 * A053868  Numbers whose sum of proper divisors is odd.
 * https://oeis.org/A053868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 7937, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9901 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose sum of proper divisors is odd.
   Equivalent characterization: 
   - Odd non‑square numbers, 
   - Even perfect squares, or 
   - Twice a perfect square. 
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
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

static bool is_valid(seq_t x)
{
    /* 1 is excluded */
    if (x <= 1) return false;

    /* Odd numbers: accept only if not a square */
    if ((x & (seq_t)1) != 0) {
        seq_t sq = floor_sqrt(x);
        return (sq * sq != x);
    }

    /* Even numbers: accept if a square */
    seq_t sq = floor_sqrt(x);
    if (sq * sq == x) return true;

    /* Even numbers: accept if twice a square */
    seq_t half = x >> 1;          /* x / 2 */
    seq_t sq2 = floor_sqrt(half);
    return (sq2 * sq2 == half);
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t cur = 0;         /* last integer examined */

    while (cache_len < n) {
        /* Find the next valid integer */
        do {
            cur += 1;
        } while (!is_valid(cur));

        grow_seq(&cache, &cache_cap, cache_len + 1);
        cache[cache_len++] = cur;
    }
    return cache[n - 1];
}
