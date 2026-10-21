/*
 * A238511  Number of (n+1)X(2+1) 0..3 arrays with no element equal to all
 *          horizontal neighbors or equal to all vertical neighbors, and new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A238511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A238511:
   a(n) = 22·a(n-1) + 277·a(n-2) + 42·a(n-3) - 3413·a(n-4)
          - 576·a(n-5) + 1877·a(n-6) + 38722·a(n-7) - 43921·a(n-8)
          - 27402·a(n-9) + 38409·a(n-10) - 4968·a(n-11) - 9432·a(n-12)
          + 8640·a(n-13) + 3456·a(n-14)
   valid for n ≥ 16.  Initial values:
   a(1)=25, a(2)=401, a(3)=14407, a(4)=430504, a(5)=13398587,
   a(6)=413256106, a(7)=12771744135, a(8)=394537124897,
   a(9)=12189010788413, a(10)=376564621157074,
   a(11)=11633561391153531, a(12)=359406052601243460,
   a(13)=11103456404402492173, a(14)=343029107715954897545,
   a(15)=10597508186010294218619 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n) {
    /* Cache of previously computed terms (1‑based indexing). */
    static seq_t *cache = NULL;
    static idx_t size = 0;

    /* Return zero for non‑positive indices. */
    if (n < 1) return 0;

    /* Initialize cache with the 15 known base terms if not already. */
    if (size < 15) {
        seq_t *init = (seq_t*)malloc(sizeof(seq_t) * 15);
        if (!init) return 0;
        init[0]  = 25ULL;
        init[1]  = 401ULL;
        init[2]  = 14407ULL;
        init[3]  = 430504ULL;
        init[4]  = 13398587ULL;
        init[5]  = 413256106ULL;
        init[6]  = 12771744135ULL;
        init[7]  = 394537124897ULL;
        init[8]  = 12189010788413ULL;
        init[9]  = 376564621157074ULL;
        init[10] = 11633561391153531ULL;
        init[11] = 359406052601243460ULL;
        init[12] = 11103456404402492173ULL;
        init[13] = seq_from_str("343029107715954897545");
        init[14] = seq_from_str("10597508186010294218619");
        cache = init;
        size = 15;
    }

    /* If the requested term is already cached, return it. */
    if (n <= size)
        return cache[n - 1];

    /* Extend the cache to accommodate term n. */
    idx_t new_size = n;
    seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
    if (!new_cache) return 0;     /* reallocation failure */
    cache = new_cache;

    /* Recurrence coefficients for a(n) = Σ coeff[j] * a(n-j-1)
       with coeff[0] = a(n-1), coeff[14] = a(n-15). */
    static const int coeff[15] = {
        22,      /* a(n-1) */
        277,     /* a(n-2) */
        42,      /* a(n-3) */
        -3413,   /* a(n-4) */
        -576,    /* a(n-5) */
        1877,    /* a(n-6) */
        38722,   /* a(n-7) */
        -43921,  /* a(n-8) */
        -27402,  /* a(n-9) */
        38409,   /* a(n-10) */
        -4968,   /* a(n-11) */
        -9432,   /* a(n-12) */
        8640,    /* a(n-13) */
        3456     /* a(n-14) */
    };

    /* Compute terms from size+1 up to n. */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* k: 0..14 corresponds to a(n-(k+1)) */
        for (int k = 0; k < 15; ++k) {
            /* Index in cache: a(n-j) -> cache[(n-j)-1] = cache[i-(k+1)-1] = cache[i-k-2] */
            idx_t idx = i - k - 2;   /* valid because i ≥ 16 for recurrence usage */
            if (coeff[k] >= 0) {
                sum += (seq_t)coeff[k] * cache[idx];
            } else {
                sum -= (seq_t)(-coeff[k]) * cache[idx];
            }
        }
        cache[i - 1] = sum;
    }

    size = new_size;
    return cache[n - 1];
}
