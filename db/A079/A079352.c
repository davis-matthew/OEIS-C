/*
 * A079352  a(1)=1, then a(n)=3*a(n-1) if n is already in the sequence,
 *          a(n)=2*a(n-1) otherwise.
 * https://oeis.org/A079352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n)=3*a(n-1) if n occurs among a(1)..a(n-1),
   otherwise a(n)=2*a(n-1). */

#include <stdlib.h>
#include <stdbool.h>

/* Cached sequence values (index 1..cache_len are valid) */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Strictly increasing list of terms generated so far */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_count = 0;

/* Binary search for a value in the terms array.
   Returns true if the value 'x' is already present. */
static bool term_present(idx_t x)
{
    seq_t key = (seq_t)x;
    size_t lo = 0, hi = terms_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (terms[mid] < key)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < terms_count && terms[lo] == key);
}

/* Ensure the terms array has enough capacity for 'need' elements. */
static void grow_terms(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = p;
    terms_cap = new_cap;
}

/* Make sure cache contains values up to and including index 'n'.
   Terms are generated on the fly and stored in the 'terms' array. */
static void ensure_cache_size(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Allocate/resize the cache array. */
    size_t new_len = (size_t)n + 1;            /* indices 0..n */
    seq_t *p = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache = p;
    idx_t old_len = cache_len;
    cache_len = n;

    for (idx_t i = old_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            bool mult3 = term_present(i);
            cache[i] = cache[i - 1] * (mult3 ? (seq_t)3 : (seq_t)2);
        }
        /* Store the generated term for future membership checks. */
        grow_terms(terms_count + 1);
        terms[terms_count++] = cache[i];
    }
}

seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n < 1, return 0 to keep type consistency */
        return (seq_t)0;
    ensure_cache_size(n);
    return cache[n];
}
