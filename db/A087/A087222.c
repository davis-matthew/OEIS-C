/*
 * A087222  G.f. satisfies A(x) = 1 + x*A(x)*f(x)^3, where f(x) = Sum_{k>=0}
 *          x^((4^k-1)/3).
 * https://oeis.org/A087222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for n>0 a(n)=∑_{k=0}^{n-1} a(k)*g[n-1-k] where g(m) =
 *            #{(s_i,s_j,s_k) : s_t=(4^t-1)/3, s_i+s_j+s_k=m}
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static state to allow arbitrary order of calls */
    static seq_t *a_arr   = NULL;   /* array of computed a values   */
    static unsigned *g_arr = NULL;  /* array of g values for current limit */
    static idx_t a_len = 0;          /* number of a values currently stored (indices 0 .. a_len-1) */
    static idx_t max_n = 0;          /* maximum n for which a values have been computed */

    /* initialise if first call */
    if (a_len == 0) {
        a_arr = (seq_t*)malloc(sizeof(seq_t));
        if (!a_arr) abort();
        a_arr[0] = (seq_t)1;
        a_len = 1;
        max_n = 0;
    }

    /* if requested n is larger than we have computed, extend */
    if (n > max_n) {
        size_t new_a_len = (size_t)n + 1;            /* need indices 0..n */
        seq_t *tmp_a = (seq_t*)realloc(a_arr, new_a_len * sizeof(seq_t));
        if (!tmp_a) abort();
        a_arr = tmp_a;

        /* recompute g up to index n-1 */
        size_t g_len = (size_t)n; /* indices 0..n-1 */
        unsigned *tmp_g = (unsigned*)calloc(g_len, sizeof(unsigned));
        if (!tmp_g) abort();
        free(g_arr);
        g_arr = tmp_g;

        /* collect values s_k = (4^k-1)/3 that are <= n-1 */
        uint64_t pow4 = 1;
        uint64_t s_vals[64];
        size_t s_cnt = 0;
        while (1) {
            uint64_t s = (pow4 - 1) / 3;
            if (s >= g_len) break;
            s_vals[s_cnt++] = s;
            /* multiply by 4, safe for small k */
            if (pow4 > ((uint64_t) -1) >> 2) break; /* overflow guard */
            pow4 <<= 2;
        }

        /* compute g as number of ordered triples of s_vals summing to each index */
        for (size_t i = 0; i < s_cnt; ++i)
            for (size_t j = 0; j < s_cnt; ++j)
                for (size_t k = 0; k < s_cnt; ++k) {
                    size_t sum = s_vals[i] + s_vals[j] + s_vals[k];
                    if (sum < g_len)
                        ++g_arr[sum];
                }

        /* compute missing a values */
        for (idx_t idx = a_len; idx <= n; ++idx) {
            seq_t total = 0;
            for (idx_t k = 0; k < idx; ++k) {
                unsigned coeff = g_arr[(size_t)(idx - 1 - k)];
                total += mul_small(a_arr[(size_t)k], coeff);
            }
            a_arr[(size_t)idx] = total;
        }
        a_len = new_a_len;
        max_n = n;
    }

    return a_arr[(size_t)n];
}
