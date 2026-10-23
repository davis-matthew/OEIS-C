/*
 * A174213  Natural numbers n such that the concatenation n//1331 is a prime
 *          number.
 * https://oeis.org/A174213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth natural number m such that the decimal
 *            concatenation of m and 1331 (i.e., m*10000 + 1331) is prime
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The following helper is part of the common utilities in the
   repository. It performs a deterministic Miller–Rabin primality
   test on an arbitrary-precision integer type `seq_t`. */
#include "../utils/is_prime_big.h"

/* ------------------------------------------------------------------ */
/*  Cache of previously found sequence values.  The cache contains the
    terms a(1), a(2), ... up to a(cache_len-1).  The index of the
    integer whose concatenation was tested last is stored in
    `last_tested`.  All data are static so the function is
    thread‑unsafe but satisfies the single‑threaded test harness. */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;          /* array of cached a(k) values */
static size_t cache_len = 0;         /* number of cached terms       */
static size_t cache_cap = 0;         /* allocated capacity            */
static idx_t last_tested = 0;        /* last integer n examined */

/* Ensure that the cache has at least `new_len` free slots.  This
   function uses realloc and grows the array geometrically. */
static void
ensure_cache_capacity(size_t new_len)
{
    if (new_len <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < new_len)
        new_cap = new_len;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In the unlikely event of an allocation failure we abort.
           The test harness does not exercise this path. */
        abort();
    }

    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Zero or negative indices are outside the defined offset.
       The behaviour is unspecified; we simply return 0. */
    if (n == 0) {
        return 0;
    }

    /* Grow the cache until it contains at least `n` terms. */
    while (cache_len < (size_t)n) {
        /* Test the next integer `m`. */
        last_tested += 1;
        idx_t m = last_tested;

        /* Compute the concatenated number m*10000 + 1331.
           All arithmetic is done in `seq_t` to avoid overflow
           for large `seq_t` types, but the calculation fits in
           64‑bit for the n values considered (tens of thousands). */
        seq_t candidate = ((seq_t)m) * (seq_t)10000u + (seq_t)1331u;

        /* If the candidate is prime, store `m` in the cache. */
        if (is_prime_big(candidate)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len] = (seq_t)m;
            cache_len += 1;
        }
    }

    /* Retrieve the requested term.  We use 0‑based indexing
       into the cached array. */
    return cache[n - 1];
}
