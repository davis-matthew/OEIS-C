/*
 * A164840  Right-truncatable squares: every prefix is square.
 * https://oeis.org/A164840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, full, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: full, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Right-truncatable squares: a(n) returned by enumerating numbers
   where every left prefix is a perfect square.  First few terms
   (offset 1): 1, 4, 9, 16, 49, 169. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
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

static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

static void build_cache(void)
{
    // Initial capacities
    cache_cap = 8;
    cache = mem_new(seq_t, cache_cap);
    if (!cache) return;                // allocation failed – give up

    size_t qcap = 8;
    seq_t *queue = mem_new(seq_t, qcap);
    if (!queue) { mem_free(cache); cache = NULL; return; }

    size_t head = 0, tail = 0;

    // Seed with single‑digit perfect squares
    queue[tail++] = 1;
    queue[tail++] = 4;
    queue[tail++] = 9;

    while (head < tail) {
        seq_t cur = queue[head++];

        // Store current square
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
            if (!tmp) break;            // realloc failed – keep existing data
            cache = tmp;
        }
        cache[cache_len++] = cur;

        // Try to append each decimal digit
        for (int d = 0; d < 10; ++d) {
            seq_t next = cur * (seq_t)10 + (seq_t)d;
            seq_t r = floor_sqrt(next);
            if (r * r == next) {
                // Valid extension – enqueue for further processing
                if (tail >= qcap) {
                    qcap *= 2;
                    seq_t *tmp = mem_resize(seq_t, queue, qcap);
                    if (!tmp) break;
                    queue = tmp;
                }
                queue[tail++] = next;
            }
        }
    }

    mem_free(queue);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      // undefined for n < 1
    if (cache_len == 0) build_cache();
    if (n <= cache_len) return cache[n - 1];
    return 0;                  // no such term beyond the finite sequence
}
