/*
 * A085087  a(1) = 1; for n>1, a(n) = a(n-1)*n if n is prime, a(n) = a(n-1)/n
 *          if n is composite dividing a(n-1) else a(n) = a(n-1).
 * https://oeis.org/A085087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 60, u128 n <= 106,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   for n > 1:
   a(n) = a(n-1) * n           if n is prime,
   a(n) = a(n-1) / n           if n is composite and n divides a(n-1),
   a(n) = a(n-1)               otherwise. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

/* Memoised cache: cache[i] holds a(i+1) for i >= 0 */
static seq_t *cache      = NULL;   /* 0‑based array, type seq_t* */
static idx_t cache_len  = 0;       /* number of elements currently stored */

/* Ensure that cache[0 .. needed-1] are computed and stored.  */
static void ensure_cache(idx_t needed)
{
    if (cache_len >= needed)
        return;

    /* Allocate or grow the array */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)needed);
    if (!new_cache)
        abort();

    cache = new_cache;

    /* Compute values from cache_len+1 up to needed */
    for (idx_t i = cache_len; i < needed; ++i) {
        /* Current n = i+1 (1‑based) */
        idx_t n = i + 1;

        /* a(n-1) is stored at index i-1 */
        seq_t prev = cache[i - 1];
        seq_t val;

        if (is_prime(n)) {
            /* Prime: multiply */
            val = prev * (seq_t)n;
        } else {
            /* Composite */
            seq_t div = (seq_t)n;
            if (gcd(prev, div) == div) {
                /* n divides a(n-1): divide */
                val = prev / div;
            } else {
                /* unchanged */
                val = prev;
            }
        }
        cache[i] = val;
    }
    cache_len = needed;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1; return 0 for safety */
        return (seq_t)0;

    /* Initialise cache with a(1) */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = (seq_t)1;    /* a(1) = 1 */
    }

    /* Compute up to the requested n */
    ensure_cache(n);

    return cache[n - 1];        /* 0‑based index */
}
