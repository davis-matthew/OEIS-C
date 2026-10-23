/*
 * A183588  1/24 the number of (n+1)X4 0..3 arrays with every 2X2 subblock
 *          containing exactly three distinct values.
 * https://oeis.org/A183588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A183588 (OEIS)
   Recurrence:
   a(n)=51*a(n-1)+274*a(n-2)-10688*a(n-3)-31344*a(n-4)+590588*a(n-5)+
        1621408*a(n-6)-5825824*a(n-7)-7929216*a(n-8)+17443840*a(n-9)+
        6201344*a(n-10)-11993088*a(n-11)
   Initial values a(1)…a(11) are hard‑coded from the OEIS entry. */

/* Only stdlib and seq_from_str are required for big‑int support. */
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* The 11 initial terms of the sequence (as decimal strings). */
    static const char *const init_str[12] = {
        NULL, /* 0 unused */
        "520",
        "27380",
        "1427084",
        "74491726",
        "3887550226",
        "202888369698",
        "10588548225946",
        "552606468835138",
        "28840014212057954",
        "1505133360863436226",
        "78551501834521854626"
    };

    /* Precomputed values are stored in a static cache. */
    static seq_t *cache   = NULL;   /* cache[1] = a(1), … */
    static idx_t cache_lim = 0;     /* largest n already computed    */
    static int  init_done  = 0;     /* whether init_str have been
                                      converted to seq_t          */

    /* Convert init_str to seq_t values and initialise cache for
       indices 1..11 on first call. */
    if (!init_done) {
        /* Allocate space for indices 0..11 (index 0 unused). */
        cache = (seq_t *)calloc(12, sizeof(seq_t));
        if (cache == NULL) return 0;            /* memory error */
        for (idx_t i = 1; i <= 11; ++i)
            cache[i] = seq_from_str(init_str[i]); /* big‑int from string */
        cache_lim = 11;
        init_done = 1;
    }

    if (n <= cache_lim) {
        /* Return a pre‑computed initial term. */
        return cache[n];
    }

    /* Extend the cache to compute up to n. */
    idx_t new_lim = n;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_lim + 1) * sizeof(seq_t));
    if (new_cache == NULL) return 0;          /* memory error */
    cache = new_cache;

    /* Coefficients for the recurrence (a(n‑1) … a(n‑11)). */
    static const long long coeff[11] = {
        51LL,      274LL,   -10688LL,  -31344LL,
        590588LL,  1621408LL,
        -5825824LL, -7929216LL,
        17443840LL, 6201344LL, -11993088LL
    };

    /* Compute missing terms with the recurrence. */
    for (idx_t i = cache_lim + 1; i <= new_lim; ++i) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;

        for (int j = 0; j < 11; ++j) {
            idx_t idx = i - (idx_t)(j + 1);          /* a(i‑(j+1)) */
            long long c = coeff[j];
            long long mag = (c > 0) ? c : -c;
            seq_t temp = cache[idx] * (seq_t)mag;
            if (c > 0)
                pos_sum += temp;
            else
                neg_sum += temp;
        }
        cache[i] = pos_sum - neg_sum;               /* pos_sum >= neg_sum */
    }

    cache_lim = new_lim;
    return cache[n];
}
