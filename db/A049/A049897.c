/*
 * A049897  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 4.
 * https://oeis.org/A049897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   log2_floor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence: a(1) = 1, a(2) = 1, a(3) = 4.
   For n >= 4:
       p  = ⌊log₂(n-2)⌋           (unique integer with 2ᵖ < n-1 ≤ 2ᵖ⁺¹)
       m  = 2ᵖ⁺¹ + 2 - n
       a(n) = a(1)+…+a(n-1) – a(m)
*/
/* Include standard headers */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/log2_floor_u64.h"

/* --------- static cache --------- */
static seq_t *cache = NULL;     /* array of a(i) values, index 0 => a(1) */
static size_t cap = 0;          /* allocated capacity of cache (in terms) */
static size_t max_n = 0;        /* greatest n computed so far */
static seq_t total = 0;         /* sum   a(1)+…+a(max_n)            */

/* --------- main function --------- */
seq_t a(idx_t n)
{
    /* immediate return for the three known terms */
    if (n <= 3) {
        if (cache == NULL) {
            /* initialise the static cache for the first call */
            cache = (seq_t*)malloc(3 * sizeof(seq_t));
            if (cache == NULL) return 0; /* allocation failure */
            cache[0] = (seq_t)1; /* a(1) */
            cache[1] = (seq_t)1; /* a(2) */
            cache[2] = (seq_t)4; /* a(3) */
            cap   = 3;
            max_n = 3;
            total = (seq_t)1 + (seq_t)1 + (seq_t)4;
        }
        return cache[(size_t)(n - 1)];
    }

    /* ensure cache is allocated (first call for n>3) */
    if (cache == NULL) {
        size_t init_cap = (size_t)4;
        if (init_cap < (size_t)n) init_cap = (size_t)n;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return 0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cap   = init_cap;
        max_n = 3;
        total = (seq_t)1 + (seq_t)1 + (seq_t)4;
    }

    /* grow the cache if needed */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* compute all missing a(i) up to n */
    for (idx_t i = (idx_t)(max_n + 1); i <= n; ++i) {
        /* current sum of a(1)…a(i-1) is in 'total' */
        int p = log2_floor_u64(i - 2);               /* i≥4, so i-2≥2 */
        seq_t pow2 = ((seq_t)1) << (p + 1);          /* 2ᵖ⁺¹ */
        seq_t m = pow2 + (seq_t)2 - (seq_t)i;        /* m ≥ 1 */
        seq_t val = total - cache[(size_t)(m - 1)];   /* recurrence */
        cache[(size_t)(i - 1)] = val;
        total += val;
        max_n = i;
    }

    return cache[(size_t)(n - 1)];
}
