/*
 * A228937  Expansion of
 *          (1+2*x+30*x^2+13*x^3-13*x^5-30*x^6-2*x^7-x^8)/(1+2*x^4+x^8).
 * https://oeis.org/A228937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228937: Expansion of (1+2*x+30*x^2+13*x^3-13*x^5-30*x^6-2*x^7-x^8)/(1+2*x^4+x^8).
 * Recurrence: a(0)=1, a(1)=2, a(2)=30, a(3)=13, a(4)=-2, a(5)=-17, a(6)=-90, a(7)=-28,
 * a(8)= 2;  for n > 8,  a(n) = -2*a(n-4) - a(n-8).  */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* cache of computed terms */
static size_t cache_cap = 0;       /* current capacity of the cache */

/* ensure the cache covers indices 0..n, expanding if necessary */
static void ensure_cache(idx_t n)
{
    if (n < (idx_t)cache_cap)
        return;                    /* already covered */

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;              /* double until large enough */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Out of memory – aborting is acceptable for this build. */
        abort();
    }
    cache = new_cache;
    /* initialize new slots (not strictly needed until computed). */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = (seq_t)0;
    cache_cap = new_cap;
}

/* The required function. */
seq_t a(idx_t n)
{
    /* Compute up to n if not already done. */
    ensure_cache(n);

    /* Fill in missing values up to n. */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        if (i <= 8 && cache[i] == (seq_t)0) {            /* initial terms */
            switch ((int)i) {
                case 0: cache[i] = (seq_t)1;  break;
                case 1: cache[i] = (seq_t)2;  break;
                case 2: cache[i] = (seq_t)30; break;
                case 3: cache[i] = (seq_t)13; break;
                case 4: cache[i] = (seq_t)-2; break;
                case 5: cache[i] = (seq_t)-17;break;
                case 6: cache[i] = (seq_t)-90;break;
                case 7: cache[i] = (seq_t)-28;break;
                case 8: cache[i] = (seq_t)2;  break;
            }
        } else if (i > 8 && cache[i] == (seq_t)0) {     /* recurrence */
            /* a(i) = -2*a(i-4) - a(i-8) */
            seq_t t = (seq_t)2 * cache[i - 4];
            cache[i] = -t - cache[i - 8];
        }
    }

    return cache[n];
}
