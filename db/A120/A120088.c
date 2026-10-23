/*
 * A120088  Numerators of partial sums of a series for sqrt(2).
 * https://oeis.org/A120088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of 1 + (1/2) * Σ_{k=0..n} (-1)^k *
 *            Catalan(k)/4^k
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/catalan.h"   /* seq_t catalan(idx_t k); */
#include "../utils/pow2.h"      /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n) {
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    /* state of the running sum S_n = curr_num / 2^curr_exp */
    static seq_t curr_num  = 0;
    static idx_t  curr_exp = 0;

    /* Return cached value if already computed */
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* Extend cache to size n+1 */
    size_t new_len = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out‑of‑memory guard */
    } else {
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }

    /* Start from the last computed index (or 0 if none) */
    idx_t start_k = (idx_t)cache_len;
    /* If this is the very first call, initialise from scratch */
    if (cache_len == 0) {
        curr_num = 0;
        curr_exp = 0;
    }

    for (idx_t k = start_k; k <= n; ++k) {
        seq_t cat        = catalan(k);
        idx_t term_exp   = 2 * k;

        if (curr_exp >= term_exp) {
            /* same denominator after scaling up the Catalan */
            idx_t diff      = curr_exp - term_exp;
            seq_t shift     = pow2(diff);          /* 2^(diff) */
            seq_t cat_shift = cat * shift;
            if ((k & 1) == 0)
                curr_num += cat_shift;
            else
                curr_num -= cat_shift;
        } else {
            /* need to scale current sum up */
            idx_t diff      = term_exp - curr_exp;
            seq_t shift     = pow2(diff);
            curr_num = curr_num * shift;
            if ((k & 1) == 0)
                curr_num += cat;
            else
                curr_num -= cat;
            curr_exp = term_exp;
        }

        /* remove trailing powers of two from the numerator */
        while ((curr_num & 1) == 0) {
            curr_num >>= 1;
            curr_exp--;
        }

        /* a(k) = curr_num + 2^(curr_exp + 1) */
        seq_t two = pow2(curr_exp + 1);
        cache[(size_t)k] = curr_num + two;
    }

    cache_len = new_len;
    return cache[(size_t)n];
}
