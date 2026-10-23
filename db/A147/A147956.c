/*
 * A147956  All positive integers that are not multiples of any Fibonacci
 *          numbers >= 2.
 * https://oeis.org/A147956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer not divisible by any *
 *            Fibonacci number >= 2. * * Implementation: a growing sieve marks
 *            numbers that are multiples of * Fibonacci numbers and caches the
 *            surviving numbers. The function * can be called in any order; it
 *            lazily extends the sieve and * cache until the...
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static state ----------------------------------------------------------- */
static idx_t          max_limit = 0;          /* current sieve limit          */
static unsigned char *candidate = NULL;       /* mark of valid numbers        */
static idx_t          *fib       = NULL;       /* Fibonacci numbers list       */
static size_t         fib_cnt   = 0;          /* number used in fib[]         */
static size_t         fib_cap   = 0;          /* allocated size of fib[]      */
static seq_t          *cache     = NULL;       /* cached sequence values       */
static size_t         cache_len = 0;          /* number of cached terms       */
static size_t         cache_cap = 0;          /* allocated size of cache[]    */

/* ensure that fib contains all Fibonacci numbers up to new_limit -------- */
static void extend_limit(idx_t new_limit)
{
    if (new_limit <= max_limit) return;
    idx_t old_limit = max_limit;

    /* grow candidate array */
    candidate = (unsigned char*)realloc(candidate,
                                        (size_t)new_limit + 1);
    if (!candidate) abort();
    for (idx_t i = old_limit + 1; i <= new_limit; ++i) candidate[i] = 1;
    candidate[0] = 0;          /* never used */

    /* initialise fib list if needed */
    if (fib_cnt < 2) {
        fib_cap = 10;
        fib = (idx_t*)realloc(fib, fib_cap * sizeof(idx_t));
        if (!fib) abort();
        fib[0] = 1;
        fib[1] = 1;
        fib_cnt = 2;
    }

    /* extend fib until last entry >= new_limit */
    while (fib[fib_cnt - 1] < new_limit) {
        if (fib_cnt == fib_cap) {
            fib_cap *= 2;
            fib = (idx_t*)realloc(fib, fib_cap * sizeof(idx_t));
            if (!fib) abort();
        }
        fib[fib_cnt] = fib[fib_cnt - 1] + fib[fib_cnt - 2];
        fib_cnt++;
    }

    /* mark multiples of Fibonacci numbers >= 2 */
    for (size_t fi = 0; fi < fib_cnt; ++fi) {
        idx_t f = fib[fi];
        if (f < 2) continue;
        if (f > new_limit) break;
        idx_t start = (old_limit / f + 1) * f;
        if (start == 0) start = f;
        for (idx_t k = start; k <= new_limit; k += f)
            candidate[k] = 0;
    }

    /* append new valid numbers to cache */
    for (idx_t i = old_limit + 1; i <= new_limit; ++i) {
        if (candidate[i]) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_len++] = (seq_t)i;
        }
    }

    max_limit = new_limit;
}

/* extend cache until it contains at least n terms ------------------------ */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;
    idx_t target = max_limit;
    if (target == 0) target = n * 10 + 100;       /* initial guess */
    while (cache_len < n) {
        target = target ? target * 2 : n * 10 + 100;
        extend_limit(target);
    }
}

/* requested function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for 0; guard */
    ensure_cache(n);
    return cache[n - 1];
}
