/*
 * A039363  Numbers whose base-9 representation has the same nonzero number of
 *          0's and 7's.
 * https://oeis.org/A039363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-9 representation has equal non‑zero counts
 *          of the digits 0 and 7.  a(1) = 63, a(2) = 88, ...
 *
 * This routine builds the sequence on demand, caching the discovered
 * terms in a static array that can hold up to 20,000 entries.  The
 * routine works for any seq_t implementation (small unsigned types or
 * arbitrary‑precision classes) because it only reads from and writes to
 * the cache array using assignment.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static bool is_valid(uint64_t x)
{
    uint64_t t = x;
    int c0 = 0, c7 = 0;
    while (t > 0) {
        uint32_t d = t % 9U;
        if (d == 0U) ++c0;
        else if (d == 7U) ++c7;
        t /= 9U;
    }
    return (c0 > 0 && c0 == c7);
}

#define CACHE_CAP 20000   /* must exceed expected n (≥10000) */

static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static uint64_t last_candidate = 0;

seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    while (cache_len < target) {
        ++last_candidate;
        if (is_valid(last_candidate)) {
            cache[cache_len++] = (seq_t)last_candidate;
        }
    }
    return cache[n - 1];
}
