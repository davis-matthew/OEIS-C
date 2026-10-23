/*
 * A165366  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A165366:  number of reduced words of length n in a Coxeter
   group with 24 generators where S_i^2 = (S_i S_j)^9 = I.
   The sequence satisfies the linear recurrence
       a(n) = 22*a(n-1) + 22*a(n-2) + … + 22*a(n-8) – 253*a(n-9)
   for n ≥ 10, with initial values a(0)…a(9) given below. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 10 pre‑computed initial values from the OEIS entry */
    static const seq_t init[10] = {
        (seq_t)1,
        (seq_t)24,
        (seq_t)552,
        (seq_t)12696,
        (seq_t)292008,
        (seq_t)6716184,
        (seq_t)154472232,
        (seq_t)3552861336,
        (seq_t)81715810728,
        (seq_t)1879463646468ULL   /* a(9) */
    };

    /*  static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of entries stored   */
    static size_t cache_cap = 0;   /* allocated capacity          */

    /* ensure cache can hold up to max(n,9)+1 entries */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        if (cache_cap < 10) cache_cap = 10;        /* at least the 10 initials */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return (seq_t)0;         /* allocation failure */
        cache_len = 0;
    }
    else if (cache_cap <= (size_t)n) {               /* grow if needed         */
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to the requested index */
    while (cache_len <= (size_t)n) {
        seq_t val;
        if (cache_len < 10) {                       /* initial values */
            val = init[cache_len];
        } else {                                    /* recurrence for n ≥ 10 */
            seq_t sum = 0;
            for (idx_t i = 1; i <= 8; ++i)          /* a(n-1)…a(n-8)   */
                sum += cache[cache_len - i];
            val = 22 * sum - 253 * cache[cache_len - 9];
        }
        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n];
}
