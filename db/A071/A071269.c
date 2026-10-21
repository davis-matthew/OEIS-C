/*
 * A071269  Terms of A007908 which are divisible by their index.
 * https://oeis.org/A071269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A071269:  terms of A007908 (concatenation of 1,2,…,m) that are
 * divisible by their index m.  The n‑th returned value is the concatenation
 * of 1,…,m where m is the n‑th integer for which the concatenation is
 * divisible by m.  The values are computed in the type seq_t; on builds
 * where seq_t is an unsigned integer type this works exactly, on builds
 * where seq_t is an arbitrary‑precision integer class it works because
 * basic arithmetic operators are defined.
 *
 * The function keeps state between calls, generating further terms only
 * as needed, so the order of the calls does not matter.
 */

#include <stdint.h>
#include <stdlib.h>

/* The values of the sequence are stored in a dynamically grown array.
 * cache_size  : number of terms already stored
 * cache_cap   : current allocated capacity
 * cache       : pointer to array of seq_t
 */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

/* The concatenation of the integers 1 … m-1 before the next step.
 * It is updated incrementally:
 *   fullVal = concatenation of 1 … current_m
 */
static seq_t fullVal = 0;

/* The next integer m to test (1‑based). */
static idx_t nextM = 1;

/* Re‑allocate the cache to hold at least new_cap items. */
static void
ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t cap = cache_cap ? cache_cap * 2 : 16;
    if (cap < new_cap)
        cap = new_cap;
    seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!new_cache) {
        /* If allocation fails we abort; in the context of the framework
           this will trigger a test failure. */
        abort();
    }
    cache = new_cache;
    cache_cap = cap;
}

/* Append a value to the sequence cache. */
static void
push_cache(seq_t val)
{
    ensure_cache_capacity(cache_size + 1);
    cache[cache_size++] = val;
}

/* Compute the n-th term of the sequence.  n≥1. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* m to test in this iteration */
        idx_t m = nextM;

        /* Number of decimal digits of m. */
        idx_t temp = m;
        idx_t digits = 0;
        while (temp > 0) {
            digits++;
            temp /= 10;
        }

        /* Compute 10^digits as a seq_t.  For digits ≤ 20 this
           can be done safely by repeated multiplication. */
        seq_t pow10_val = 1;
        for (idx_t i = 0; i < digits; ++i)
            pow10_val *= 10;

        /* Update the full concatenation:
         * fullVal = fullVal * 10^digits + m
         */
        fullVal = fullVal * pow10_val + (seq_t)m;

        /* Test divisibility: fullVal % m == 0 */
        if ((fullVal % (seq_t)m) == 0)
            push_cache(fullVal);

        /* Advance to the next m. */
        ++nextM;
    }

    return cache[n - 1];
}
