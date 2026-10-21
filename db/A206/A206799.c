/*
 * A206799  Based on an erroneous version of A008614.
 * https://oeis.org/A206799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 50, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-3) + a(n-4) - a(n-10) - a(n-11) + a(n-14) 
 *              - d(n),
 * where d(n) is the coefficient of x^n in
 *   -4 - x + 2 x^3 + x^4 - 2 x^5 - 2 x^6 + 2 x^7 + 3 x^8 + 2 x^9 - 3 x^11.
 * The recurrence holds for all n ≥ 0 with a(m) = 0 for m < 0.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;          /* array of computed values */
static idx_t cache_size = 0;         /* current capacity of cache   */
static idx_t last_computed = 0;      /* largest n for which cache[n] is valid */

/* grow cache to hold at least index n (idx_t) */
static void ensure_size(idx_t n) {
    if (cache == NULL) {
#ifdef __cplusplus
        cache = new seq_t[16]();          /* value‐initialized to zero */
#else
        cache = (seq_t*)calloc(16, sizeof(seq_t));
#endif
        cache_size = 16;
        cache[0] = 4;                    /* a(0) = 4 */
        last_computed = 0;
    }
    while (n >= cache_size) {
        size_t new_size = cache_size * 2;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_size]();  /* zero‑initalized */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        seq_t *new_cache = realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        memset(new_cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        cache = new_cache;
#endif
        cache_size = new_size;
    }
}

/* the required OEIS sequence */
seq_t a(idx_t n) {
    ensure_size(n);
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t term = 0;
            if (i >= 3)  term += cache[i - 3];
            if (i >= 4)  term += cache[i - 4];
            if (i >= 10) term -= cache[i - 10];
            if (i >= 11) term -= cache[i - 11];
            if (i >= 14) term += cache[i - 14];

            /* subtract d(i) */
            seq_t coeff = 0;
            switch (i) {
                case 0:  coeff = (seq_t)-4;  break;
                case 1:  coeff = (seq_t)-1;  break;
                case 3:  coeff = (seq_t) 2;  break;
                case 4:  coeff = (seq_t) 1;  break;
                case 5:  coeff = (seq_t)-2;  break;
                case 6:  coeff = (seq_t)-2;  break;
                case 7:  coeff = (seq_t) 2;  break;
                case 8:  coeff = (seq_t) 3;  break;
                case 9:  coeff = (seq_t) 2;  break;
                case 11: coeff = (seq_t)-3;  break;
                /* for all other i, coeff remains 0 */
            }
            term -= coeff;
            cache[i] = term;
        }
        last_computed = n;
    }
    return cache[(size_t)n];
}
