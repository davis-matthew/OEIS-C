/*
 * A115819  Integers n such that 4*n XOR 15*n = 19*n.
 * https://oeis.org/A115819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   integers n such that (4*n) XOR (15*n) = 19*n. a(1) = 0, a(2) =
 *            5, a(3) = 10,
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Test the defining condition using 64‑bit arithmetic. */
static inline bool satisfies_uint64(uint64_t n)
{
    uint64_t four     = n * 4ULL;
    uint64_t fifteen  = n * 15ULL;
    uint64_t nineteen = n * 19ULL;
    return (four ^ fifteen) == nineteen;
}

#define MAX_CACHE 200000   /* sufficient for the first 10000-ish terms */

/* Cached terms. */
static seq_t cache[MAX_CACHE];
static size_t cache_size   = 0;   /* number of stored terms */
static idx_t  next_candidate = 0; /* next integer to test */

/* Grow the cache until it contains at least 'need' terms. */
static void ensure_cached(size_t need)
{
    while (cache_size < need)
    {
        /* Find the next integer that satisfies the condition. */
        while (!satisfies_uint64((uint64_t)next_candidate))
            ++next_candidate;

        /* Store the term if there is space in the static array. */
        if (cache_size < MAX_CACHE)
            cache[cache_size] = (seq_t)next_candidate;

        ++cache_size;
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) is undefined; return 0 as guard */
    ensure_cached((size_t)n);
    return cache[(size_t)n - 1];
}
