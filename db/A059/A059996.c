/*
 * A059996  Concatenate Catalan numbers (A000108).
 * https://oeis.org/A059996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat(C_0 , C_1 , … , C_n) where C_k are Catalan numbers
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation uses dynamic caching of the partial concatenations
    a(0), a(1), … and of the Catalan numbers.  Catalan numbers are
    generated iteratively via  
          C_0 = 1
          C_k = C_{k-1} * 2 * (2*k-1) / (k+1)    (exact division)   */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Helper: number of decimal digits of a non‑zero seq_t value. */
static int digits(seq_t x)
{
    int d = 0;
    while (x > 0) {
        x /= 10;
        ++d;
    }
    return d;
}

/* Helper: 10^e as seq_t.  e is small enough that a loop is cheap. */
static seq_t pow10_int(idx_t e)
{
    seq_t r = 1;
    while (e--) r *= 10;
    return r;
}

/* Cached results */
static seq_t *a_cache   = NULL;   /* a(n) */
static seq_t *cat_cache = NULL;   /* Catalan numbers C_n */
static idx_t   cache_sz = 0;      /* size of cached arrays (n+1 for index n) */

/* Ensure the cache contains entries up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache_sz > 0 && n < cache_sz) return;   /* already present */

    /* Resize arrays to hold indices 0 … n */
    size_t new_sz = (size_t)(n + 1);
    a_cache   = (seq_t *)realloc(a_cache,   new_sz * sizeof(seq_t));
    cat_cache = (seq_t *)realloc(cat_cache, new_sz * sizeof(seq_t));
    if (!a_cache || !cat_cache) abort();        /* out of memory */

    /* Initialise from cache_sz up to n */
    idx_t i;
    if (cache_sz == 0) {
        /* start with n = 0 */
        cat_cache[0] = (seq_t)1;
        a_cache[0]   = (seq_t)1;
        i = 1;
    } else {
        i = cache_sz;
    }

    for (; i <= n; ++i) {
        /* Catalan recurrence: C_i = C_{i-1} * 2*(2*i-1) / (i+1) */
        seq_t numer = cat_cache[i - 1] * (seq_t)(2 * (2 * (idx_t)(i) - 1));
        cat_cache[i] = numer / (seq_t)(i + 1);

        /* Number of decimal digits of C_i */
        int d = digits(cat_cache[i]);

        /* Build the new concatenation:
           a_i = a_{i-1} * 10^d + C_i  */
        seq_t pow10_d = pow10_int((idx_t)d);
        a_cache[i] = a_cache[i - 1] * pow10_d + cat_cache[i];
    }

    cache_sz = new_sz;
}

/* Public API required by OEIS framework */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
