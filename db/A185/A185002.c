/*
 * A185002  Numbers k such that P(k^2+1) > P((k+1)^2+1) where P(n) (A006530)
 *          is the largest prime factor of n.
 * https://oeis.org/A185002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k such that P(k²+1) > P((k+1)²+1), *
 *            where P(x) is the largest prime factor of x
 * Range:     u8 n <= 123, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 78 terms through n = 4880 (2026-11-03)
 * Helpers:   largest_prime_factor, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/largest_prime_factor.h"
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

static seq_t *seq_cache = NULL;   /* cache of the sequence values          */
static size_t seq_len  = 0;       /* number of values stored so far        */
static size_t seq_cap  = 0;       /* current allocation capacity           */
static uint64_t last_k_checked = 0;  /* k of the last examined integer        */

/* Grow the cache to at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (seq_cache == NULL || need > seq_cap)
        grow_seq(&seq_cache, &seq_cap, need);
}

/* Return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety – the official offset is 1 */

    /* Generate sequence elements until we have at least n of them. */
    while (seq_len < (size_t)n)
    {
        ++last_k_checked;
        uint64_t k = last_k_checked;
        uint64_t p1 = largest_prime_factor(k * k + 1);
        uint64_t p2 = largest_prime_factor((k + 1) * (k + 1) + 1);
        if (p1 > p2)
        {
            ensure_cache_cap(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)k;   /* conversion works for all seq_t types */
        }
    }
    return seq_cache[n - 1];
}
