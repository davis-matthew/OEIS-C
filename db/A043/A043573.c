/*
 * A043573  Numbers whose base-2 representation has exactly 6 runs.
 * https://oeis.org/A043573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(n) -> the n-th integer whose binary representation
   contains exactly 6 runs.
   The algorithm scans integers in order, counting runs in each
   binary representation and storing the numbers that satisfy the
   condition.  It keeps a cache so that repeated calls with arbitrary
   indices are O(1) after the initial pass. */
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Utility: count the number of runs in the binary representation of
   a non‑negative integer x.
   A run is a maximal sequence of equal consecutive bits.
   Complexity: O(number of bits of x). */
static int count_runs(seq_t x)
{
    int runs = 0;
    bool prev = false;
    bool first = true;
    seq_t y = x;
    while (y > 0) {
        bool bit = (y & 1) != 0;
        if (first) {
            runs = 1;
            prev = bit;
            first = false;
        } else if (bit != prev) {
            ++runs;
            prev = bit;
        }
        y >>= 1;
    }
    return runs;
}

/* ------------------------------------------------------------------ */
/* Cache data.  The cache grows only when more terms are requested. */
static seq_t *cache = NULL;
static idx_t cached_len = 0;   /* number of cached terms */
static seq_t next_candidate = 0;   /* next integer to test */

/* Ensure the cache contains at least n terms. */
static void extend_cache(idx_t n)
{
    if (n <= cached_len) return;

    /* Allocate a new array large enough for all terms that will be
       added during this call.  Avoid reallocating for every single
       new term to reduce overhead. */
    size_t new_len = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failure */
    cache = tmp;

    /* Scan integers until we have cached_len == n. */
    while (cached_len < n) {
        ++next_candidate;              /* move to the next integer */

        if (count_runs(next_candidate) == 6) {
            cache[cached_len] = next_candidate;
            ++cached_len;
        }
    }
}

/* ------------------------------------------------------------------ */
/* External entry point.  n is 1‑based. */
seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n - 1];
}
