/*
 * A229508  Number of defective 3-colorings of an n X 6 0..2 array connected
 *          diagonally and antidiagonally with exactly one mistake, and colors
 *          introduced in row-major 0..2 order.
 * https://oeis.org/A229508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Empirical recurrence (valid for n > 13):
        a(n) = 50*a(n‑1)  – 715*a(n‑2) + 324*a(n‑3) + 50175*a(n‑4)
           – 180630*a(n‑5) – 940491*a(n‑6) + 4556250*a(n‑7)
           + 2329155*a(n‑8) – 27398736*a(n‑9) + 13286025*a(n‑10)
           + 47829690*a(n‑11) – 43046721*a(n‑12)

   Initial terms:
        a(1)=0, a(2)=7680, a(3)=228096, a(4)=6298560, a(5)=162171072,
        a(6)=4032737280, a(7)=97662620160, a(8)=2320483572864,
        a(9)=54321921368064, a(10)=1256772873968640,
        a(11)=28798595015489856, a(12)=654681579415674048,
        a(13)=14783463288895694400
*/

#include <stdlib.h>

/* Initial 13 terms (may overflow for very small seq_t; the function uses
   upfront the values only for n <= 13). */
static const seq_t init[13] = {
    0ULL,
    7680ULL,
    228096ULL,
    6298560ULL,
    162171072ULL,
    4032737280ULL,
    97662620160ULL,
    2320483572864ULL,
    54321921368064ULL,
    1256772873968640ULL,
    28798595015489856ULL,
    654681579415674048ULL,
    14783463288895694400ULL
};

/* Coefficients of the recurrence (12 terms). */
static const signed long long coeff[12] = {
    50LL, -715LL, 324LL, 50175LL, -180630LL,
    -940491LL, 4556250LL, 2329155LL, -27398736LL,
    13286025LL, 47829690LL, -43046721LL
};

/* Compute a(n) for n ≥ 1 using memoized recurrence. */
seq_t a(idx_t n)
{
    /* Hash‑based cache of computed terms. */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of terms stored */

    if (n == 0) return 0;   /* offset is 1 */

    /* Minimum cache size: we always keep the first 13 terms. */
    size_t need_cap = (size_t)n;
    if (need_cap < 13) need_cap = 13;

    /* Allocate or grow cache if necessary. */
    if (cache == NULL) {
        cache_cap = need_cap;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (need_cap > cache_cap) {
        cache = (seq_t *)realloc(cache, need_cap * sizeof(seq_t));
        cache_cap = need_cap;
    }

    /* Load the first 13 terms into cache on first use. */
    if (cache_len == 0) {
        for (size_t i = 0; i < 13; ++i) cache[i] = init[i];
        cache_len = 13;
    }

    /* Compute missing terms up to n. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 13) {           /* already loaded from init */
            val = init[i - 1];
        } else {
            /* Recurrence: a(i) = sum_{k=1}^{12} coeff[k-1] * a(i-k)   */
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            for (int k = 0; k < 12; ++k) {
                seq_t term = cache[i - k - 2];   /* a(i-(k+1)) */
                unsigned long long abs_coef =
                    (coeff[k] >= 0) ? (unsigned long long)coeff[k]
                                    : (unsigned long long)(-coeff[k]);
                seq_t prod = term * (seq_t)abs_coef;
                if (coeff[k] >= 0)
                    pos_sum += prod;
                else
                    neg_sum += prod;
            }
            val = pos_sum - neg_sum;  /* pos_sum >= neg_sum is guaranteed */
        }
        cache[i - 1] = val;
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
