/*
 * A098523  Expansion of (1+x^2)/(1-x-x^5) = (1+x^2)/((1-x+x^2)*(1-x^2-x^3)).
 * https://oeis.org/A098523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-5); a(0)=1, a(1)=1, a(2)=2, a(3)=2, a(4)=2
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A098523.  Uses memoisation so that any call
   order is supported efficiently. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* cached sequence values               */
static idx_t cache_len = 0;   /* number of values currently stored     */
static idx_t cache_cap = 0;   /* allocated capacity of cache array      */

seq_t a(idx_t n)
{
    /* Ensure cache is large enough */
    if (cache_len <= n) {
        /* expand capacity if needed */
        if (cache_cap < n + 1) {
            idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
            while (new_cap <= n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();  /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise base cases if cache is empty */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;  /* a(0) */
            cache_len = 1;
        }

        /* Fill cache up to required index n */
        while (cache_len <= n) {
            idx_t i = cache_len;
            seq_t val;
            if (i < 5) {
                /* known initial values */
                switch (i) {
                    case 0: val = (seq_t)1; break;
                    case 1: val = (seq_t)1; break;
                    case 2: val = (seq_t)2; break;
                    case 3: val = (seq_t)2; break;
                    case 4: val = (seq_t)2; break;
                    default: val = (seq_t)0; /* unreachable */
                }
            } else {
                val = cache[i - 1] + cache[i - 5];
            }
            cache[i] = val;
            cache_len = i + 1;
        }
    }

    return cache[n];
}
