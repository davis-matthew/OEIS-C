/*
 * A042086  Numerators of continued fraction convergents to sqrt(567).
 * https://oeis.org/A042086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042086: Numerators of continued fraction convergents to sqrt(567)
   Recurrence: a(n) = 4048*a(n-6) - a(n-12)  for n >= 12
   Initial values: a(0)=23, a(1)=24, a(2)=119, a(3)=381, a(4)=1643, a(5)=2024,
                    a(6)=94747, a(7)=96771, a(8)=481831, a(9)=1542264,
                    a(10)=6650887, a(11)=8193151
   The function caches computed terms and works for any order of calls. */

#include <stdlib.h>
#include <string.h>

/* Computes the nth term of OEIS sequence A042086. */
seq_t a(idx_t n)
{
    /* Static cache and bookkeeping. */
    static seq_t *cache = NULL;   /* dynamic array of terms */
    static size_t cap = 0;        /* allocated size */
    static idx_t last = 0;        /* largest index computed in cache */

    /* Seed values (required to start recurrence). */
    static const seq_t init[12] = {
        23, 24, 119, 381, 1643, 2024,
        94747, 96771, 481831, 1542264,
        6650887, 8193151
    };

    /* --------------------------------------------------------------- */
    /* Initialise cache with the 12 seed values. */
    if (cache == NULL) {
        cap = 12;                       /* at least 12 slots */
        cache = (seq_t *)malloc(cap * sizeof(*cache));
        if (cache == NULL) return 0;    /* allocation failure */
        memcpy(cache, init, sizeof(init));  /* copy seeds */
        last = 11;                      /* highest seed index */
    }

    /* --------------------------------------------------------------- */
    /* Expand cache if required. */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(*cache));
        if (tmp == NULL) return 0;  /* allocation failure */
        /* zero‑initialize newly added part (not strictly needed) */
        memset(tmp + cap, 0, (newcap - cap) * sizeof(*cache));
        cache = tmp;
        cap = newcap;
    }

    /* --------------------------------------------------------------- */
    /* Compute missing terms using the recurrence. */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* a(i) = 4048*a(i-6) - a(i-12) */
            cache[i] = 4048 * cache[i - 6] - cache[i - 12];
        }
        last = n;
    }

    return cache[n];
}
