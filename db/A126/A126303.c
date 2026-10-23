/*
 * A126303  a(n) = number of nodes with odd distance to the root in the n-th
 *          plane general tree encoded by A014486(n). Both internal and
 *          terminal nodes (leaves) are counted.
 * https://oeis.org/A126303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nodes with odd distance to the root in the *
 *            n-th plane tree encoded by a Dyck word. The sequence is obtained
 *            by * listing all non‑negative integers whose binary
 *            representation is a * balanced parentheses sequence; the n-th
 *            such integer is used to trace * a rooted ordered tree an...
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   isDyck, oddDepthCount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isDyck.h"
#include "../utils/oddDepthCount.h"

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* capacity of cache array */
    static uint64_t next_int    = 1;/* next integer to test for Dyck word */

    /* First call: initialise cache with a(0)=0. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory – should not happen */
        cache[0] = 0;                  /* a(0) = 0 (empty tree) */
        cache_size = 1;
    }

    /* Grow the cache until it contains the n-th value. */
    if ((size_t)n >= cache_size) {
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            /* Find the next Dyck word. */
            while (1) {
                if (isDyck(next_int)) {
                    seq_t val = oddDepthCount(next_int);
                    /* Grow the array if necessary. */
                    if (cache_size == cache_cap) {
                        cache_cap *= 2;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                        if (!cache) return 0; /* allocation failed */
                    }
                    cache[cache_size++] = val;
                    ++next_int;
                    break;               /* found the next term */
                }
                ++next_int;
            }
        }
    }

    return cache[(size_t)n];
}
