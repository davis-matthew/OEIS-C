/*
 * A197052  Number of nX6 0..4 arrays with each element equal to the number
 *          its horizontal and vertical zero neighbors.
 * https://oeis.org/A197052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1..19) are given.
   For n > 19:
   a(n) = 2*a(n-1)+7*a(n-2)+9*a(n-3)-15*a(n-4)-38*a(n-5)+45*a(n-6)
          -16*a(n-7)-91*a(n-9)+117*a(n-10)-63*a(n-11)+52*a(n-12)
          -19*a(n-13)+5*a(n-14)+8*a(n-15)-10*a(n-16)+4*a(n-17)-4*a(n-18)
   offset 1 (a(1) first).   */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* initial values a(1)..a(19) */
    static const seq_t init_vals[19] = {
        (seq_t)5,          /* a(1) */
        (seq_t)16,         /* a(2) */
        (seq_t)78,         /* a(3) */
        (seq_t)274,        /* a(4) */
        (seq_t)1132,       /* a(5) */
        (seq_t)4468,       /* a(6) */
        (seq_t)17742,      /* a(7) */
        (seq_t)70616,      /* a(8) */
        (seq_t)281202,     /* a(9) */
        (seq_t)1117442,    /* a(10) */
        (seq_t)4448148,    /* a(11) */
        (seq_t)17693664,   /* a(12) */
        (seq_t)70390082,   /* a(13) */
        (seq_t)280040518,  /* a(14) */
        (seq_t)1114106280, /* a(15) */
        (seq_t)4432207738, /* a(16) */
        (seq_t)17633023628,/* a(17) */
        (seq_t)70149839190,/* a(18) */
        (seq_t)279079644000/* a(19) */
    };

    /* coefficients for a(n-1)..a(n-18) */
    static const int coeffs[18] = {
        2,  7,   9,  -15, -38, 45, -16, 0,
       -91, 117, -63, 52, -19, 5,  8, -10, 4, -4
    };

    static seq_t *cache = NULL;     /* array of computed values, 0‑based */
    static idx_t cached = 0;        /* number of values stored in cache */
    static idx_t allocated = 0;     /* capacity of cache */

    /* allocate or grow cache to hold at least the requested term */
    if (cache == NULL) {
        allocated = (n > 19 ? n : 19);
        cache = (seq_t*)calloc(allocated, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        memcpy(cache, init_vals, sizeof(init_vals));
        cached = 19;
        if (cached > n) cached = n;                  /* keep usable size */
    } else if (n > allocated) {
        seq_t *new_cache = (seq_t*)calloc(n, sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;      /* allocation failure */
        memcpy(new_cache, cache, cached * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        allocated = n;
    }

    /* if the term was already computed, return it */
    if (n <= cached)
        return cache[n - 1];

    /* compute missing terms up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        if (i <= 19) {                 /* defensive: copy initial values */
            cache[i - 1] = init_vals[i - 1];
            continue;
        }

        seq_t sum = 0;
        for (int k = 0; k < 18; ++k) {
            idx_t idx = i - (k + 1);   /* index of a(n-(k+1)) */
            seq_t prod = cache[idx - 1] *
                         (seq_t)(coeffs[k] < 0 ? -coeffs[k] : coeffs[k]);
            if (coeffs[k] >= 0)
                sum += prod;
            else
                sum -= prod;
        }
        cache[i - 1] = sum;
    }
    cached = n;
    return cache[n - 1];
}
