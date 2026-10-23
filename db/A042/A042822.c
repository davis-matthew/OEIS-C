/*
 * A042822  Numerators of continued fraction convergents to sqrt(942).
 * https://oeis.org/A042822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042822: Numerators of continued‑fraction convergents to sqrt(942) */
/* Recurrence (for n ≥ 16):  a(n) = 212266*a(n-8) − a(n-16)          */
/* Initial values (n = 0 … 15) are generated from the continued‑fraction of sqrt(942) */
/* a(0)=30, a(1)=31, a(2)=92, a(3)=399, a(4)=8072, a(5)=32687, a(6)=73446, a(7)=106133,*/
/* a(8)=6441426, a(9)=6547559, a(10)=19536544, a(11)=84693735, a(12)=1713411244, a(13)=6938338711,*/
/* a(14)=15590088666, a(15)=22528427377. */

#include <stdlib.h>

/* Periodic part of the continued fraction expansion of sqrt(942)
   after the leading integer part a0 = 30. */
static const seq_t cf_period[8] = {
    (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)20,
    (seq_t)4, (seq_t)2, (seq_t)1, (seq_t)60
};

/* Cached values of the sequence to allow non‑monotonic calls. */
static seq_t *cache      = NULL;   /* pointer to array of terms          */
static idx_t cache_size  = 0;      /* number of terms currently cached   */
static idx_t cache_cap   = 0;      /* allocated capacity (in terms)      */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold up to term n. */
    if (n >= cache_size) {
        idx_t needed = n + 1; /* data for indices 0 … n inclusive */

        if (needed > cache_cap) {
            /* Grow the allocation (next power of two ≥ needed). */
            idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
            while (new_cap < needed) new_cap <<= 1;

            seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;        /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms one by one. */
        for (idx_t idx = cache_size; idx <= n; ++idx) {
            seq_t val;

            if (idx == 0) {
                /* The very first convergent: a0 = 30. */
                val = (seq_t)30;
            } else if (idx == 1) {
                /* a1 = cf_period[0] = 1, needs the virtual p[-1] = 1. */
                val = cf_period[0] * cache[0] + (seq_t)1;
            } else if (idx < 16) {
                /* Use the recurrence for convergents of sqrt(942). */
                seq_t ai = cf_period[(idx - 1) % 8];
                val = ai * cache[idx - 1] + cache[idx - 2];
            } else {
                /* Linear recurrence derived from the period. */
                val = (seq_t)212266 * cache[idx - 8] - cache[idx - 16];
            }

            cache[idx] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
