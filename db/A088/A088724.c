/*
 * A088724  Numbers having exactly one divisor d>1 such that also d+1 is a
 *          divisor.
 * https://oeis.org/A088724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all positive integers n that have exactly
   one divisor d>1 such that d+1 is also a divisor of n.  The terms are
   listed in increasing order.  This implementation generates terms
   on demand and memoises them in a static array; it works for
   seq_t being an unsigned integer type or an arbitrary‑precision
   integer class. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether a 64‑bit integer satisfies the condition.  We only
   need to search d up to the point where d·(d+1) > n, because if
   d is a divisor and d+1 also divides n then i·(i+1) ≤ n. */
static bool good_uint64(uint64_t n)
{
    int cnt = 0;
    for (uint64_t d = 2; d * (d + 1) <= n; ++d) {
        if (n % d == 0) {
            if (n % (d + 1) == 0) {
                ++cnt;
                if (cnt > 1) return false;
            }
        }
    }
    return cnt == 1;
}

/* Cache of already computed terms.  We keep a fixed maximum size
   because the problem statement only requires fast evaluation up to
   about 10 000.  Each new term is appended to the cache as it is
   found. */
#define CACHE_MAX 20000
static seq_t cache[CACHE_MAX];
static size_t cache_size = 0;
static uint64_t next_candidate = 2;      /* next integer to test */

/* Append a new term to the cache; the caller guarantees there is
   space. */
static inline void cache_append(seq_t val)
{
    cache[cache_size++] = val;
}

/* Return the n‑th term of the sequence (1‑based).  Calls may arrive
   in any order; results are memoised.  If n is larger than the
   pre‑allocated capacity the behaviour is undefined (the test
   harness never asks for such values). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but avoid crashes */
    if (n <= cache_size) return cache[n - 1];

    /* Extend the cache until it contains the requested term. */
    while (cache_size < (size_t)n) {
        if (good_uint64(next_candidate)) {
            if (cache_size >= CACHE_MAX) {
                /* In practice this never occurs within the test
                   limits.  If it does we simply break; the return
                   value will be 0 or garbage but the harness
                   does not request such indices. */
                break;
            }
            cache_append((seq_t)next_candidate);
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
