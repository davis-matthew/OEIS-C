/*
 * A054393  Number of permutations with certain forbidden subsequences.
 * https://oeis.org/A054393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies (-n+3)*a(n) + (10*n-33)*a(n-1) +
 *            5*(-7*n+24)*a(n-2) + 2*(22*n-63)*a(n-3) + 2*(5*n-78)*a(n-4) +
 *            (-55*n+357)*a(n-5) + (22*n-135)*a(n-6) + 3*(-n+6)*a(n-7) = 0
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=14,
 *            a(5)=42, a(6)=132
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  computed_upto = 0;

    /* initialise cache and base cases */
    if (cache == NULL) {
        size_t init_cap = (n > 6) ? (size_t)(n + 1) : 7;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)14;
        cache[5] = (seq_t)42;
        cache[6] = (seq_t)132;
        cache_cap = init_cap;
        computed_upto = 6;
    }

    /* grow cache if required */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t m = computed_upto + 1; m <= n; ++m) {
        if (m <= 6) {          /* base values already stored */
            continue;
        }

        /* accumulate positive and negative parts separately to avoid
           overflow of signed intermediates when seq_t is unsigned */
        seq_t sum_pos = (seq_t)0;
        seq_t sum_neg = (seq_t)0;
        seq_t coeff;

        /* (10*n-33)*a(n-1)   (always positive for n>=7) */
        coeff = (seq_t)(10*m - 33);
        sum_pos += coeff * cache[m - 1];

        /* 5*(-7*n+24)*a(n-2)  (negative coefficient) */
        coeff = (seq_t)(5*(7*m - 24));
        sum_neg += coeff * cache[m - 2];

        /* 2*(22*n-63)*a(n-3)  (positive) */
        coeff = (seq_t)(2*(22*m - 63));
        sum_pos += coeff * cache[m - 3];

        /* 2*(5*n-78)*a(n-4)  (sign depends on n) */
        int64_t tmp4 = 5*(int64_t)m - 78;
        if (tmp4 >= 0) {
            coeff = (seq_t)(2*tmp4);
            sum_pos += coeff * cache[m - 4];
        } else {
            coeff = (seq_t)(2*(-tmp4));
            sum_neg += coeff * cache[m - 4];
        }

        /* (-55*n+357)*a(n-5)  (negative for n>=7) */
        coeff = (seq_t)(55*m - 357);
        sum_neg += coeff * cache[m - 5];

        /* (22*n-135)*a(n-6)  (positive) */
        coeff = (seq_t)(22*m - 135);
        sum_pos += coeff * cache[m - 6];

        /* 3*(-n+6)*a(n-7)  (negative for n>=7) */
        coeff = (seq_t)(3*(m - 6));
        sum_neg += coeff * cache[m - 7];

        seq_t numerator = sum_pos - sum_neg;          /* exact division */
        seq_t denom = (seq_t)(m - 3);
        cache[m] = numerator / denom;
    }

    computed_upto = n;
    return cache[n];
}
