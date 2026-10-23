/*
 * A166739  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^12 = I.
 * https://oeis.org/A166739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  Sequence A166739 – derived from its generating function
 *      a(0..11) = {1, 46, 2070, 93150, 4191750, 188628750,
 *                   8488293750, 381973218750, 17188794843750,
 *                   773495767968750, 34807309558593750, 1566328930136718750}
 *  Recurrence for n ≥ 12:
 *      a(n) = 44 * (a(n-1)+…+a(n-11)) - 990 * a(n-12) + (n==12 ? 1 : 0)
 *  (The extra 1 comes from the numerator term t¹² in the generating function.)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t cache_alloc = 0;
    static idx_t cache_used  = 0;

    /* constants used in the recurrence */
    static const seq_t C44  = (seq_t)44;
    static const seq_t C990 = (seq_t)990;
    static const seq_t C1   = (seq_t)1;

    /* first 12 terms of the sequence (indices 0..11) */
    static const seq_t init[12] = {
        (seq_t)1,
        (seq_t)46,
        (seq_t)2070,
        (seq_t)93150,
        (seq_t)4191750,
        (seq_t)188628750,
        (seq_t)8488293750,
        (seq_t)381973218750,
        (seq_t)17188794843750,
        (seq_t)773495767968750,
        (seq_t)34807309558593750,
        (seq_t)1566328930136718750
    };

    /* enlarge cache if necessary */
    if (n + 1 > cache_alloc) {
        idx_t new_alloc = n + 1;
        if (new_alloc < 13u) new_alloc = 13u;        /* a few spare slots */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_alloc);
        if (!tmp) return (seq_t)0;                  /* out of memory */
        cache       = tmp;
        cache_alloc = new_alloc;
    }

    /* initialize cached values if not yet done */
    if (cache_used < 12u) {
        for (idx_t i = 0; i < 12u; ++i) cache[i] = init[i];
        cache_used = 12u;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_used; i <= n; ++i) {
        /* compute the sum of the previous 11 terms */
        seq_t sum = (seq_t)0;
        for (idx_t j = i - 11u; j < i; ++j) sum += cache[j];

        /* recurrence */
        cache[i] = C44 * sum - C990 * cache[i - 12u];
        if (i == 12u) cache[i] += C1;  /* extra +1 term when n==12 */
    }

    cache_used = n + 1u;
    return cache[n];
}
