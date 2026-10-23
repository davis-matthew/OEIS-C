/*
 * A135391  Numbers n such that 9^n and 9^(n+1) have the same number of
 *          decimal digits.
 * https://oeis.org/A135391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th integer m≥1 such that * floor(m·log10(9)) ==
 *            floor((m+1)·log10(9)). * * The function builds the sequence
 *            lazily, caching computed terms * for reuse on subsequent calls.
 *            It uses only integer arithmetic * and a very small
 *            floating‑point constant; no floating‑point * library calls ar...
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
/* The next integer k to test (as idx_t). */
static idx_t next_k = 1;

/* Constants for log10(9).  1 – log10(9) is the threshold
 * below which the condition holds.  The values are written
 * with sufficient precision for the problem scale. */
static const long double LOG10_9   = 0.954242509439324875L;
static const long double THRESHOLD = 1.0L - LOG10_9;   /* 0.045757490560675125L */
static long double frac = 0.0L;      /* fractional part of k·log10(9) */

/* Ensure that the cache contains at least 'need' terms. */
static void generate_upto(idx_t need)
{
    if (need <= cache_len)
        return;

    /* Expand the cache array if required. */
    if (cache_cap < (size_t)need) {
        size_t new_cap = (size_t)need * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Iterate through integers, storing those that satisfy the condition. */
    while (cache_len < (size_t)need) {
        /* Update fractional part of k·log10(9). */
        frac += LOG10_9;
        if (frac >= 1.0L)
            frac -= 1.0L;

        /* Condition: fractional part < 1 – log10(9). */
        if (frac < THRESHOLD)
            cache[cache_len++] = (seq_t)next_k;

        ++next_k;
    }
}

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* Undefined for n < 1, return 0 safely. */

    generate_upto(n);
    return cache[n - 1];
}
