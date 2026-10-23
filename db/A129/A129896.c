/*
 * A129896  a(1)=1. a(n) = a(n-1) + number of Fibonacci numbers among the
 *          first (n-1) terms of the sequence.
 * https://oeis.org/A129896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1
   a(n) = a(n‑1) + F(n‑1)   (n > 1)
   where F(k) is the number of Fibonacci numbers among a(1), … , a(k).  */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Test whether x is a Fibonacci number. */
static inline bool is_fibonacci(seq_t x)
{
    if (x == (seq_t)0) return true;
    seq_t five = (seq_t)5;
    seq_t s1 = five * x * x + (seq_t)4;
    seq_t r1 = floor_sqrt(s1);
    if (r1 * r1 == s1) return true;
    seq_t s2 = five * x * x - (seq_t)4;
    seq_t r2 = floor_sqrt(s2);
    return r2 * r2 == s2;
}

/* Sequence a(n).  Memoised, 1‑based indexing. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* out of bounds – undefined */

    static seq_t *cache = NULL;          /* array of computed values (1‑based) */
    static size_t cache_cap = 0;         /* highest index for which cache is valid */
    static size_t fib_cnt = 0;           /* Fibonacci numbers seen in a[1..cache_cap] */

    size_t need = (size_t)n;

    /* First invocation – allocate and compute the whole sequence up to n. */
    if (cache == NULL) {
        cache = (seq_t *)realloc(NULL, (need + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */

        cache[1] = (seq_t)1;
        fib_cnt = is_fibonacci(cache[1]) ? 1 : 0;
        cache_cap = 1;

        for (size_t i = 2; i <= need; ++i) {
            cache[i] = cache[i - 1] + (seq_t)fib_cnt;
            if (is_fibonacci(cache[i])) ++fib_cnt;
        }
        cache_cap = need;
    }
    /* Extend cache if required. */
    else if (need > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, (need + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure */
        cache = new_cache;

        for (size_t i = cache_cap + 1; i <= need; ++i) {
            cache[i] = cache[i - 1] + (seq_t)fib_cnt;
            if (is_fibonacci(cache[i])) ++fib_cnt;
        }
        cache_cap = need;
    }

    return cache[need];
}
