/*
 * A053335  a(n) contains n digits (either '3' or '6') and is divisible by
 *          2^n.
 * https://oeis.org/A053335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 6; a(n) = d * 10^(n-1) + a(n-1) where d = 6 if a(n-1) is
 *            divisible by 2^n, otherwise d = 3. The recurrence follows
 *            a(n)=a(n-1)+10^(n-1)*(6-3*[a(n-1)/2^(n-1) mod 2])
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following function computes A053335 for any n ≥ 1.  It caches previously
   computed values and reuses them for subsequent calls.
   It works with the generic integer type seq_t (8–128 bits or arbitrary-precision)
   and the index type idx_t.  No external libraries are required beyond the
   standard C library. */

#include <stdlib.h>   /* for calloc, realloc */

/* Forward declaration of the function */
seq_t a(idx_t n);

/* Static storage for memoisation */
static seq_t *cache = NULL;    /* cache[i] holds a(i)  (1‑based) */
static idx_t  cache_size = 0;  /* highest n stored so far */
static seq_t  pow10_val = 0;   /* current value of 10^cache_size */

/* Compute A053335 */
seq_t a(idx_t n)
{
    /* allocate/resize cache if necessary */
    if (cache == NULL) {
        cache = (seq_t*)calloc(2, sizeof(seq_t)); /* indices 0 and 1 */
        cache_size = 0;
    }
    if (n > cache_size) {
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {            /* base case */
                cache[1] = 6;
                pow10_val = 10;         /* 10^1 */
                cache_size = 1;
                continue;
            }
            /* Determine the leading digit for a(i) */
            seq_t q = (cache[i - 1] >> (i - 1)); /* q = a(i-1)/2^(i-1) */
            seq_t d = ((q & 1) == 0) ? 6 : 3;    /* 6 if q even, else 3 */

            /* Build a(i) = d * 10^(i-1) + a(i-1) */
            cache[i] = d * pow10_val + cache[i - 1];

            /* Update pow10_val to 10^i for next iteration */
            pow10_val *= 10;
        }
        cache_size = n;
    }
    return cache[n];
}
