/*
 * A055608  Arrays of dumbbells.
 * https://oeis.org/A055608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = coefficient of x^(n-1) in (1+x)^4 / ((1-x)^4 *
 *            (1-x-x^2)^5)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 21, u64 n <= 58, u128 n <= 142,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Using the linear recurrence derived from the rational generating
    function:   a(k) = - Σ_{j=1}^{14} q_j · a(k-j)  + p_k ,
    where q_j are the coefficients of Q(x)= (1-x)^4(1-x-x^2)^5
    and p_k is the coefficient of x^k in the numerator P(x)=(1+x)^4
    (p_k=0 for k>4).  The function works for any order of evaluation
    and memoises the values. */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /*--- 0‑based index ---*/
    if (n == 0) return (seq_t)0;           /* undefined, return 0 */
    idx_t idx = n - 1;                     /* 0‑based */

    /*---  recursion coefficients ---*/
    static int64_t coeff_q[15];            /* Q(x) coefficients, q0 … q14 */
    static int    coeff_p[5];              /* P(x) coefficients, p0 … p4 */
    static int    init_flag = 0;           /* initialisation flag */

    /*--- memoisation cache ---*/
    static seq_t *cache = NULL;
    static size_t cache_len = 0;           /* number of elements stored */
    static size_t cache_cap = 0;           /* allocated capacity        */

    /*--- initialise coefficients once ---*/
    if (!init_flag) {
        /* numerator P(x) = (1 + x)^4 = 1 + 4x + 6x^2 + 4x^3 + x^4 */
        coeff_p[0] = 1;
        coeff_p[1] = 4;
        coeff_p[2] = 6;
        coeff_p[3] = 4;
        coeff_p[4] = 1;

        /* denominator (1-x)^4  ->  [1, -4, 6, -4, 1]            */
        int64_t poly1[5] = { 1, -4, 6, -4, 1 };

        /* (1 - x - x^2)^5  */
        int64_t base[3] = { 1, -1, -1 };          /* 1 - x - x^2 */
        int64_t poly[11] = { 0 };                 /* degree 10 */
        int64_t tmp[11] = { 0 };
        int deg = 0;
        poly[0] = 1;                               /* (1 - x - x^2)^0 */
        for (int m = 1; m <= 5; ++m) {
            for (int i = 0; i <= deg; ++i)
                for (int j = 0; j < 3; ++j)
                    tmp[i + j] += poly[i] * base[j];
            for (int i = 0; i < 11; ++i) poly[i] = 0;
            for (int i = 0; i < 11; ++i) poly[i] = tmp[i];
            for (int i = 0; i < 11; ++i) tmp[i] = 0;
            deg += 2;
        }

        /* convolution poly1 * poly  = Q(x) */
        int64_t polyQ[15] = { 0 };                /* degree 14 */
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j <= 10; ++j)
                polyQ[i + j] += poly1[i] * poly[j];

        /* store Q coefficients */
        for (int i = 0; i < 15; ++i)
            coeff_q[i] = polyQ[i];

        init_flag = 1;
    }

    /*--- grow cache if needed ---*/
    if (cache_cap <= idx) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= idx) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                              /* realloc failed */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /*--- compute terms up to idx --*/
    for (size_t k = cache_len; k <= (size_t)idx; ++k) {
        seq_t sum = (seq_t)0;
        /* accumulate Σ_{j=1}^{14} qj * a(k-j) */
        for (int j = 1; j <= 14 && k >= (size_t)j; ++j) {
            int64_t c = coeff_q[j];
            if (c == 0) continue;
            seq_t prod = mul_small(cache[k - j], (unsigned int)(c > 0 ? c : -c));
            if (c > 0) sum = sum + prod; else sum = sum - prod;
        }
        /* p_k term */
        seq_t pk = (k <= 4) ? (seq_t)coeff_p[(int)k] : (seq_t)0;
        /* recurrence: a_k = -sum + p_k */
        cache[k] = pk - sum;
    }
    cache_len = idx + 1;
    return cache[idx];
}
