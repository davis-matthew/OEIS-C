/*
 * A058901  Inconsummate numbers in base 5: no number is this multiple of the
 *          sum of its digits (in base 5).
 * https://oeis.org/A058901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 9547, u32 n <= 9547, u64 n <= 9547,
 *            u128 n <= 9547, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A058901 – Inconsummate numbers in base 5.
   A number n belongs to the sequence iff there does not exist a positive
   integer k satisfying
        sumBase5Digits(n*k) = k.
   The implementation generates terms lazily, testing each candidate
   n by brute force and caching successful values. */

#include <stdlib.h>
#include <stdbool.h>

/* Check whether n has a multiple n*k whose base‑5 digit sum equals k. */
static bool has_solution(idx_t n)
{
    for (idx_t k = 1; ; ++k) {
        unsigned __int128 prod = (unsigned __int128)n * k;

        /* Compute the base‑5 digit sum of prod. */
        seq_t sum = 0;
        unsigned digits = 0;
        unsigned __int128 t = prod;
        while (t > 0) {
            sum += (unsigned long long)(t % 5);
            t   /= 5;
            ++digits;
        }
        if (digits == 0) digits = 1;   /* prod == 0, impossible for n>0 */

        if (sum == k)
            return true;               /* forbidden multiple found */

        /* When k is larger than any possible digit sum at this point,
           further k cannot satisfy the equation. */
        if (k > 4u * digits + 4u)
            return false;
    }
}

/* Lazy cache of computed terms. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 0;

/* Ensure that cache has capacity for at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t old_cap = cache_cap;
    while (cache_cap < new_cap)
        cache_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    /* If the cache was previously empty, initialize length. */
    if (old_cap == 0) cache_len = 0;
}

/* Main routine required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;           /* undefined, return 0 as safe fallback */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        ++last_checked;
        if (!has_solution(last_checked)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
