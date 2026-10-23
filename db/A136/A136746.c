/*
 * A136746  G.f.: (z^12+1-z^11-z^10+z^8-z^6+z^5-z^3+z)/((z+1)*(z-1)^2).
 * https://oeis.org/A136746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for sequence A136746:
   a(0)=1, a(1)=2, a(2)=3.
   For n≥3:
       a(n)=a(n-1)+a(n-2)-a(n-3)
            with the following adjustments to match the G.F.:
            n=3  : subtract 1
            n=5  : add 1
            n=6  : subtract 1
            n=8  : add 1
            n=10 : subtract 1
            n=11 : subtract 1
            n=12 : add 1
   These are the only n for which the constant term of the numerator
   contributes non‑zero. This recurrence is derived from the rational
   generating function 
      (z^12+1-z^11-z^10+z^8-z^6+z^5-z^3+z)/((z+1)*(z-1)^2). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static idx_t  computed_upto = 0;   /* last computed index */

    /* ensure we have storage for indices up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_size) {
        size_t new_size = needed;
        if (new_size < 16) new_size = 16;   /* avoid tiny reallocs */
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_size];
#else
        new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
#endif
        /* copy existing values */
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        /* free old storage */
#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_size = new_size;
    }

    /* initialise base values once */
    if (computed_upto < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        computed_upto = 2;
    }

    if (n <= computed_upto)
        return cache[(size_t)n];

    /* compute missing values up to n */
    for (idx_t k = computed_upto + 1; k <= n; ++k) {
        seq_t val = cache[(size_t)(k-1)] + cache[(size_t)(k-2)];
        val -= cache[(size_t)(k-3)];
        switch (k) {
            case 3:  val -= (seq_t)1; break;
            case 5:  val += (seq_t)1; break;
            case 6:  val -= (seq_t)1; break;
            case 8:  val += (seq_t)1; break;
            case 10: val -= (seq_t)1; break;
            case 11: val -= (seq_t)1; break;
            case 12: val += (seq_t)1; break;
            default: break;
        }
        cache[(size_t)k] = val;
    }
    computed_upto = n;
    return cache[(size_t)n];
}
