/*
 * A027398  Number of labeled servers of dimension 11.
 * https://oeis.org/A027398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   _compute_fk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A027398 – Number of labeled servers of dimension 11
   Formula: a(n) = n! * [x^n] exp( Σ_{i=1..11} ((1+x)^i - 1)/i )
   The recurrence used:
     a(0) = 1
     a(n) = Σ_{k=1}^{min(n,11)} binom(n-1, k-1) * f_k * a(n-k)
   where f_k = Σ_{i=k}^{11} (i-1)!/(i-k)!   (integers)
   This yields an O(n·11) algorithm using only integer arithmetic. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_compute_fk.h"

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------ */
    /* Pre‑compute the constants f_k (k = 1..11) exactly once.      */
    /* ------------------------------------------------------------ */
    static seq_t f[12] = {0};   /* f[1]..f[11] used, f[0] unused */
    static bool f_built = false;
    if (!f_built) {
        for (idx_t k = 1; k <= 11; ++k) {
            seq_t sum = 0;
            for (idx_t i = k; i <= 11; ++i) {
                seq_t term = 1;
                for (idx_t t = 1; t < k; ++t)
                    term = term * (seq_t)(i - t);
                sum += term;
            }
            f[k] = sum;
        }
        f_built = true;
    }

    /* ------------------------------------------------------------ */
    /* Dynamic cache of computed a(·) values.                       */
    /* ------------------------------------------------------------ */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t last_computed = 0;      /* highest index computed */

    if (n >= cache_size) {
        size_t new_size = cache_size ? cache_size * 2 : 1;
        while (new_size <= (size_t)n) new_size <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_size = new_size;
    }

    if (cache[0] == 0) cache[0] = 1;   /* a(0) = 1 */

    /* ------------------------------------------------------------ */
    /* Compute missing values up to n.                              */
    /* ------------------------------------------------------------ */
    for (idx_t cur = last_computed + 1; cur <= n; ++cur) {
        seq_t sum = 0;
        idx_t maxk = (cur <= 11) ? cur : 11;
        for (idx_t k = 1; k <= maxk; ++k) {
            /* binom(cur-1, k-1)   */
            seq_t comb = 1;
            for (idx_t j = 1; j < k; ++j) {
                comb = comb * (seq_t)(cur - j) / (seq_t)j; /* exact division */
            }
            seq_t term = comb * f[k] * cache[(size_t)(cur - k)];
            sum += term;
        }
        cache[(size_t)cur] = sum;
    }
    last_computed = n;
    return cache[(size_t)n];
}
