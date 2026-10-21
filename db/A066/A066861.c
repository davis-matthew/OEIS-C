/*
 * A066861  For x > 0 let f(x) = x/2 if x is even, f(x) = (3*x+1)/2 if x is
 *          odd (3x+1 or Collatz problem). Sequence gives numbers such that
 *          k-1 applications of f lead to k for some k > 0.
 * https://oeis.org/A066861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A066861 defined by the property that there exists k>0 such that
 *            f applied (k‑1) times to n equals k, where f(x)=x/2 if x is
 *            even, f(x)=(3x+1)/2 if x is odd. This implementation generates
 *            the sequence on demand by testing each integer n for the
 *            property and caching the found terms
 * Range:     u8 n <= 10, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   collatz_step
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/collatz_step.h"

/* Test whether a given n is in the sequence */
static bool in_sequence(seq_t n)
{
    seq_t current = n;
    idx_t step = 0; /* step counter starts at 0 */

    while (true) {
        /* check equality condition */
        if (current == (seq_t)step + (seq_t)1)
            return true;

        /* if we hit the cycle 1 <-> 2, no further match will occur */
        if (current == (seq_t)1 || current == (seq_t)2)
            return false;

        current = collatz_step(current);
        ++step;
    }
}

/* Cache storage for sequence terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1; /* next integer to test */

/* Append a term to the cache, growing the array as needed */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();          /* abort on memory failure */
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Generate terms until we have at least n terms in the cache */
static void ensure_n_terms(idx_t n)
{
    while ((idx_t)cache_len < n) {
        if (in_sequence((seq_t)next_candidate))
            cache_append((seq_t)next_candidate);
        ++next_candidate;
    }
}

/* Public function specified in the problem statement */
seq_t a(idx_t n)
{
    if (n == 0)
        abort(); /* offset is 1, n must be >= 1 */
    ensure_n_terms(n);
    return cache[n - 1];
}
