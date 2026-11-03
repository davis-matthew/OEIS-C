/*
 * A227073  Positive numbers without the digit 0 such that sum of cubes of the
 *          digits equals the square of the sum of the digits.
 * https://oeis.org/A227073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 260, u32 n <= 2987, u64 n <= 2987,
 *            u128 n <= 2987, big any n
 * Validated: standard, pass, 62 terms through n = 2895 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate increasing integers, keep those without a 0 digit
   whose sum of cubes of digits equals the square of the sum of digits. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

/* Test whether x satisfies the defining condition. */
static bool good(idx_t x)
{
    int sum   = 0;
    int cubes = 0;
    for (idx_t y = x; y > 0; ) {
        int d = y % 10;
        if (d == 0) return false;          /* no digit 0 */
        sum   += d;
        cubes += d * d * d;
        y    /= 10;
    }
    return cubes == sum * sum;
}

/* Grow the cache to at least n terms. */
static void ensure_terms(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 128;
        cache = alloc_seq_array(cache_cap);
    }
    for ( ; cache_len < (size_t)n; ++next_candidate) {
        if (good(next_candidate)) {
            seq_t val = (seq_t)next_candidate;
            append_candidate(&cache, &cache_len, &cache_cap, val);
        }
    }
}

/* Return the n‑th term of the sequence.  The index is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined below offset */
    ensure_terms(n);
    return cache[n - 1];
}
