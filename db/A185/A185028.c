/*
 * A185028  Numbers of the base-10 form ddd...d having Hamming weight d.
 * https://oeis.org/A185028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   Numbers of the form ddd…d (d=1…9) whose binary popcount equals
 *            d. The function generates each such number by iterating the
 *            natural sequence d, dd, ddd, … for each digit d, and merging the
 *            nine sequences into increasing order. Lazy evaluation is used:
 *            the cache grows only as far as needed for the...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*   misc utilities – independent of the type seq_t                   */
/* ------------------------------------------------------------------ */

static inline seq_t popcount_seq_t(seq_t v)
{
    unsigned int c = 0;
    while (v != 0) {
        c += (unsigned int)(v & 1);
        v >>= 1;
    }
    return (seq_t)c;
}

/* ------------------------------------------------------------------ */
/*   cache management – works both in C (uses malloc) and in C++ (uses
   new/delete).  The cache holds all terms that have already been
   generated.                                                        */
/* ------------------------------------------------------------------ */

static seq_t *cache      = NULL;
static idx_t cache_size  = 0;
static idx_t cache_cap   = 0;

static void grow_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap) return;
#ifdef __cplusplus
    /* C++: create new array, copy, delete old */
    seq_t *tmp = new seq_t[new_cap];
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        delete[] cache;
    }
    cache = tmp;
#else
    /* C: use realloc */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;          /* failure – unlikely for given limits */
    cache = tmp;
#endif
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*   main sequence function                                           */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0, return 0 */

    /* ----------------------------------------------------------------
       initialise per‑digit current numbers on first call
       ---------------------------------------------------------------- */
    static seq_t cur[10];      /* cur[d] holds the next candidate for digit d */
    static bool init = false;
    if (!init) {
        for (int d = 1; d <= 9; ++d) cur[d] = (seq_t)d;
        init = true;
    }

    /* ----------------------------------------------------------------
       grow the result cache as far as needed
       ---------------------------------------------------------------- */
    if (cache_size < n) {
        if (!cache) grow_cache(8);           /* initial capacity */
        while (cache_size < n) {
            /* find the smallest current value among the nine digits */
            int best_d = 1;
            seq_t best_val = cur[1];
            for (int d = 2; d <= 9; ++d) {
                if (cur[d] < best_val) {
                    best_d = d;
                    best_val = cur[d];
                }
            }

            /* if the candidate satisfies popcount==d, store it */
            if (popcount_seq_t(best_val) == (seq_t)best_d) {
                if (cache_size + 1 > cache_cap) {
                    grow_cache(cache_cap * 2 + 8);
                }
                cache[cache_size++] = best_val;
            }

            /* advance this digit: next repeated‑digit number */
            cur[best_d] = cur[best_d] * 10 + (seq_t)best_d;
        }
    }

    return cache[n - 1];
}
