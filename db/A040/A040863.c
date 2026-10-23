/*
 * A040863  Continued fraction for sqrt(893).
 * https://oeis.org/A040863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040863: continued fraction coefficients of sqrt(893) */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cached values for the continued‑fraction coefficients.  */
    static seq_t *cache = NULL;     /* array of coefficients */
    static size_t cache_len = 0;    /* number of stored coefficients */
    static size_t cache_cap = 0;    /* allocated capacity */
    static size_t period = 0;       /* length of the periodic part (excluding a0) */
    static bool initialized = false;

    if (!initialized) {
        /* ----------------------------------------------------------
           Phase 1 – build the list of continued‑fraction terms
           for sqrt(893).  The calculation is performed in 64‑bit
           arithmetic (idx_t) to avoid overflow, and only the final
           coefficient is cast to seq_t for storage.
           ---------------------------------------------------------- */

        const idx_t D = 893;                     /* radicand */
        /* integer part a0 = ⌊√D⌋ */
        idx_t a0 = 0;
        while ((a0 + 1) * (a0 + 1) <= D)
            a0++;

        /* Allocate an initial cache for the terms.  */
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) { /* allocation failure – terminate the program */
            abort();
        }
        cache[0] = (seq_t)a0;   /* store a0 */
        cache_len = 1;

        /* Variables governing the continued‑fraction algorithm.  */
        idx_t m = 0, d = 1;
        idx_t a_n = a0;          /* current coefficient a_n */

        /* Generate terms until the end of the period is detected.
           The period ends when the coefficient equals 2*a0.  */
        for (;;) {
            idx_t new_m = d * a_n - m;
            idx_t new_d = (D - new_m * new_m) / d;
            idx_t new_a = (a0 + new_m) / new_d;

            /* Store the new coefficient.  */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)new_a;

            /* End of the period?  */
            if (new_a == 2 * a0) {
                period = cache_len - 1;   /* exclude a0 */
                break;
            }

            /* Advance state variables.  */
            m = new_m;
            d = new_d;
            a_n = new_a;
        }

        initialized = true;
    }

    /* ----------------------------------------------------------
       Phase 2 – answer the query for the requested index n.
       ---------------------------------------------------------- */
    if (n == 0)
        return cache[0];
    if (n <= period)
        return cache[(size_t)n];

    /* For indices beyond the period, wrap around the periodic block. */
    idx_t offset = (n - 1) % (idx_t)period;      /* 0 … period-1 */
    size_t idx = 1 + (size_t)offset;             /* 1 … period */
    return cache[idx];
}
