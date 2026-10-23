/*
 * A037433  Positive numbers having the same set of digits in base 5 and base
 *          10.
 * https://oeis.org/A037433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 217, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A037433: positive numbers whose set of digits in base 10
   is identical to the set of digits in base 5.
   The function generates terms on demand, storing them in a static cache
   and returns the n-th term (1‑indexed).  */

/* required headers */
#include <stdbool.h>
#include <stdlib.h>

/* external linkage function */
seq_t a(idx_t n) {
    /* static cache for generated terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    /* the next integer to examine; start from 1 */
    static idx_t next_candidate = 1;

    /* one‑time allocation of the cache */
    if (!cache) {
        cache_cap = 64;                     /* arbitrary initial size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* ----- check decimal digits ----- */
        idx_t x = next_candidate;           /* use idx_t for arithmetic */
        unsigned decMask = 0;
        bool ok = true;

        while (x > 0) {
            unsigned d = x % 10;
            if (d >= 5) {                   /* decimal digit too large */
                ok = false;
                break;
            }
            decMask |= 1u << d;
            x /= 10;
        }

        if (ok) {
            /* ----- check base‑5 digits ----- */
            idx_t y = next_candidate;
            unsigned base5Mask = 0;

            while (y > 0) {
                unsigned d = y % 5;
                base5Mask |= 1u << d;
                y /= 5;
            }

            if (decMask == base5Mask) {      /* masks match: accept number */
                /* ensure space in cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) {                 /* allocation failed */
                        /* abort on memory failure */
                        abort();
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }

        ++next_candidate;                    /* advance to next integer   */
    }

    /* return the requested term */
    return cache[n - 1];
}
