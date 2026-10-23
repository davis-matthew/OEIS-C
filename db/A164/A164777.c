/*
 * A164777  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula/Recurrence:
   a(0) = 1
   a(1) = 9
   a(2) = 72
   a(3) = 576
   a(4) = 4608
   a(5) = 36864
   a(6) = 294912
   a(7) = 2359296
   a(8) = 1 + 7*(a(7)+a(6)+a(5)+a(4)+a(3)+a(2)+a(1)) - 28*a(0)
   For n ≥ 9:
       a(n) = 7*a(n-1)+7*a(n-2)+…+7*a(n-7) - 28*a(n-8)
*/

#include <stdlib.h>

static const seq_t init[8] = {
    (seq_t)1ULL,     /* a(0) */
    (seq_t)9ULL,     /* a(1) */
    (seq_t)72ULL,    /* a(2) */
    (seq_t)576ULL,   /* a(3) */
    (seq_t)4608ULL,  /* a(4) */
    (seq_t)36864ULL, /* a(5) */
    (seq_t)294912ULL,/* a(6) */
    (seq_t)2359296ULL/* a(7) */
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure cache holds values up to index n */
static void ensure_cache(idx_t n)
{
    if (n < 8) return;                    /* values below a(8) are in init */

    if (cache_len == 0) {                 /* first time: allocate and init */
        idx_t cap = 9;
        while (cap <= n) cap <<= 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i < 8; ++i) cache[i] = init[i];

        /* compute a(8) */
        seq_t sum = init[1] + init[2] + init[3] + init[4] +
                    init[5] + init[6] + init[7];
        cache[8] = (seq_t)1 + (seq_t)7 * sum - (seq_t)28 * init[0];

        cache_len = 9;                     /* indices 0 … 8 stored */
    }

    if (cache_len <= n) {                  /* enlarge and compute further */
        idx_t old_len = cache_len;
        idx_t cap = cache_len;
        while (cap <= n) cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));

        for (idx_t i = old_len; i <= n; ++i) {
            /* recurrence for n ≥ 9 */
            seq_t sum = 0;
            for (int k = 1; k <= 7; ++k) sum += cache[i - k];
            cache[i] = (seq_t)7 * sum - (seq_t)28 * cache[i - 8];
        }
        cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 8) return init[n];
    ensure_cache(n);
    return cache[n];
}
