/*
 * A178179  Like A177904, but start with a(1)=0, a(2)=a(3)=1.
 * https://oeis.org/A178179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Validated: partial, pass, 107 terms through n = 107 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=a(3)=1; a(n)= largest_prime_factor(a(n-1)+a(n-2)+a(n-3)) for n≥4 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

static seq_t largest_prime_factor(seq_t x)
{
    seq_t largest = 0;
    seq_t two = 2;

    /* remove factors of 2 */
    while (x % two == 0) {
        largest = two;
        x /= two;
    }

    /* odd factors */
    seq_t d = 3;
    while (d <= x / d) {
        while (x % d == 0) {
            largest = d;
            x /= d;
        }
        d += 2;
    }

    if (x > 1)
        largest = x;

    return largest;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Ensure cache has space up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;          /* one extra for 0‑index */
    if (need > cache_cap)
        grow_seq_array(&cache, &cache_cap, need);
}

/* Compute values up to n if not already done. */
static void compute_up_to(idx_t n)
{
    ensure_cache(n);
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[1] = 0;
        } else if (i == 2 || i == 3) {
            cache[i] = 1;
        } else {
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3];
            cache[i] = largest_prime_factor(sum);
        }
    }
    cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined, return 0 */
    compute_up_to(n);
    return cache[(size_t)n];
}
