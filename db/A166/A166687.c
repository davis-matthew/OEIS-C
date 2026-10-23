/*
 * A166687  Numbers of the form x^2 + y^2 + 1, x, y integers.
 * https://oeis.org/A166687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers of the form x^2 + y^2 + 1, sorted in increasing order
 *            (OEIS A166687). The function a(n) returns the nth such number
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root for 64‑bit values (floor). */
static inline uint64_t int_sqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 0xFFFFFFFFULL;   /* sqrt(2^64-1) < 2^32 */
    if (high > n) high = n;
    uint64_t ans = 0;

    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t sq  = mid * mid;              /* safe: mid <= 2^32-1 */

        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Test whether v can be written as x^2 + y^2 + 1 for integer x,y. */
static inline bool representable(seq_t v)
{
    /* work in 64‑bit arithmetic; values considered here fit in uint64_t. */
    uint64_t val = (uint64_t)v;
    if (val == 0) return false;
    uint64_t rem = val - 1;
    uint64_t maxx = int_sqrt_u64(rem);

    for (uint64_t x = 0; x <= maxx; ++x) {
        uint64_t x2  = x * x;
        if (x2 > rem) break;
        uint64_t y2  = rem - x2;
        uint64_t y   = int_sqrt_u64(y2);
        if (y * y == y2) return true;
    }
    return false;
}

/* Dynamic cache of sequence values. */
static seq_t  *seq_cache   = NULL;
static size_t  seq_len     = 0;
static size_t  seq_cap     = 0;
static seq_t   next_checked = 0;

/* Append a value to the cache, growing it if necessary. */
static void cache_append(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
        seq_t *new_ptr = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort(); /* allocation failure */
        seq_cache = new_ptr;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* Public function implementing the required prototype. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined index, return 0 for safety */

    /* Produce terms until we have at least n entries. */
    while (seq_len < (size_t)n) {
        next_checked++;
        if (representable(next_checked)) {
            cache_append(next_checked);
        }
    }
    return seq_cache[(size_t)n - 1];
}
