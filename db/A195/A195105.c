/*
 * A195105  E.g.f.: sqrt(cos(x)^6 + sin(x)^6).
 * https://oeis.org/A195105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: A(x) = sqrt( (5 + 3 cos(4x)) / 8 )
 * The e.g.f. coefficients a(n) (n ≥ 1) are the coeff. of x^(2(n-1))/(2(n-1))! 
 * in the expansion of A(x).  They satisfy
 *
 *   a(0) = 1
 *   a(n) = ( b(n) – Σ_{k=1}^{n-1} binom(n,k) a(k) a(n-k) ) / 2   (n ≥ 1)
 *
 * where b(n) is the n-th e.g.f. coefficient of A(x)^2:
 *   b(0) = 1,
 *   b(n) = 0                          (n odd),
 *   b(2m) = 3 * (–1)^m * 2^(4m–3)     (m ≥ 1).
 *
 * The sequence is independent of the input order; values are cached
 * and computed up to the maximum requested index.
 */

#include <stddef.h>
#include <stdlib.h>

/* helpers from utils */
#include "../utils/pow2.h"   /* seq_t pow2(idx_t) */

/* external linkage */
seq_t a(idx_t n)
{
    /* offset 1: a(1) corresponds to index 0 in the e.g.f. */
    if (n == 0)
        return 0;  /* undefined, but never requested */

    /* map n to coefficient index k = 2*(n-1) */
    idx_t k_needed = 2 * (n - 1);

    /* static cache of e.g.f. coefficients a_k (k = 0..max_k) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* ensure the cache is large enough */
    if (k_needed >= cache_size) {
        size_t new_cap = k_needed + 1;
        if (new_cap < 16) new_cap = 16;
        if (new_cap > cache_cap) {
            /* grow cache */
            size_t grow = cache_cap ? cache_cap * 2 : 16;
            if (grow < new_cap) grow = new_cap;
            seq_t *tmp = (seq_t*)realloc(cache, grow * sizeof(seq_t));
            if (!tmp) { /* malloc failure */
                /* as a fallback, use 64-bit integer */
                while (cache_size <= k_needed) {
                    cache[cache_size] = 0;
                    ++cache_size;
                }
                return cache[k_needed];
            }
            /* zero new entries */
            for (size_t i = cache_cap; i < grow; ++i)
                tmp[i] = 0;
            cache = tmp;
            cache_cap = grow;
        }
        /* initialize coefficients from old size up to k_needed */
        for (size_t i = cache_size; i <= k_needed; ++i) {
            if (i == 0) {
                cache[0] = 1;  /* a_0 = 1 */
            } else if (i & 1) {
                cache[i] = 0;  /* odd indices are zero */
                continue;
            } else {
                /* compute a_i for even i */
                size_t n_even = i;   /* n_even is even */
                /* rhs: b(n_even) */
                seq_t rhs;
                if (n_even == 0) {
                    rhs = 1;
                } else {
                    size_t m = n_even / 2;
                    seq_t val = 3 * pow2(4 * m - 3);  /* 3 * 2^(4m-3) */
                    if (m & 1) val = -val;            /* multiply by (–1)^m */
                    rhs = val;
                }
                /* sum over k=2..n_even-2 step 2 */
                seq_t sum = 0;
                seq_t binom = 1;  /* binom(n,0) */
                for (size_t k = 2; k <= n_even - 2; k += 2) {
                    /* compute binom(n,k) from binom(n,k-2) */
                    seq_t num = (n_even - (k - 2)) * (n_even - (k - 1));
                    seq_t den = k * (k - 1);
                    binom = binom * num / den;
                    /* term = binom * a_k * a_{n-k} */
                    sum += binom * cache[k] * cache[n_even - k];
                }
                /* a_n = (rhs - sum)/2 */
                cache[i] = (rhs - sum) / 2;
            }
        }
        cache_size = k_needed + 1;
    }

    return cache[k_needed];
}
