/*
 * A015100  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=-5.
 * https://oeis.org/A015100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(n+1)=∑_{i=0..n} (-5)^i * a(i) * a(n-i)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches computed values.  It works for built‑in
   signed integers as well as for arbitrary‑precision integer classes
   used when compiling the solution as C++.  The code deliberately
   casts the result of realloc to seq_t* so that it compiles under
   both C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>   /* for uint64_t if needed */

static seq_t *cache = NULL;        /* cached sequence values */
static  size_t cache_len = 0;      /* allocated array size (in elements) */
static  idx_t  max_computed = 0;   /* highest index that has been set */

/* ensure the cache contains at least n+1 elements */
static void ensure_cache(idx_t n)
{
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return the cache unchanged */
            return;
        }
        cache      = tmp;
        cache_len  = new_len;
    }
    /* initialise a(0) if it hasn't been set yet */
    if (cache_len > 0 && cache[0] == 0) {
        cache[0] = 1;
        max_computed = 0;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);

    if (max_computed < n) {
        /* Compute successive terms from max_computed+1 up to n */
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            idx_t m   = k - 1;      /* upper i bound for the sum */
            seq_t sum = 0;
            seq_t q_pow = 1;        /* (-5)^i  */
            for (idx_t i = 0; i <= m; ++i) {
                seq_t prod = cache[i];
                prod *= cache[m - i];
                prod *= q_pow;
                sum += prod;
                q_pow *= (seq_t)(-5);
            }
            cache[k] = sum;
        }
        max_computed = n;
    }
    return cache[n];
}
