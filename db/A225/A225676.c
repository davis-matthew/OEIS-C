/*
 * A225676  Numbers n such that A054404(n) does not equal round(n/e).
 * https://oeis.org/A225676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search; a(n) = n-th m >= 1 with A054404(m) != round(m/e),
 *            A054404 computed by argmax_{1<=r<m} (r/m)*(1/r+...+1/(m-1))
 * Range:     u8 n <= 47, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t  *A054404_cache = NULL;
static double *A054404_H = NULL;
static idx_t   A054404_cap = 0;

static void A054404_extend(idx_t n) {
    if (n <= A054404_cap) return;
    idx_t newcap = n < 2 * A054404_cap ? 2 * A054404_cap : n;
    if (!A054404_cache) A054404_cache = mem_new(seq_t, (newcap + 1));
    else A054404_cache = mem_resize(seq_t, A054404_cache, (newcap + 1));
    if (!A054404_H) A054404_H = (double *)malloc(sizeof(double) * (newcap + 1));
    else A054404_H = (double *)realloc(A054404_H, sizeof(double) * (newcap + 1));
    if (A054404_cap == 0) A054404_H[0] = 0.0;
    for (idx_t cur = A054404_cap + 1; cur <= newcap; ++cur)
        A054404_H[cur] = A054404_H[cur - 1] + 1.0 / (double)cur;
    for (idx_t cur = A054404_cap + 1; cur <= newcap; ++cur) {
        if (cur == 1) { A054404_cache[cur] = (seq_t)0; continue; }
        double best_val = -1.0;
        idx_t best_r = 1;
        double Hn1 = A054404_H[cur - 1];
        for (idx_t r = 1; r <= cur - 1; ++r) {
            double val = (double)r * (Hn1 - A054404_H[r - 1]);
            if (val > best_val) { best_val = val; best_r = r; }
        }
        A054404_cache[cur] = (seq_t)best_r;
    }
    A054404_cap = newcap;
}

static idx_t A054404_small(idx_t n) {
    A054404_extend(n);
    return (idx_t)(unsigned long long)A054404_cache[n];
}

static idx_t *terms_arr = NULL;
static idx_t terms_cnt = 0, terms_cap = 0, next_m = 1;

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    const double E = 2.718281828459045235360287;
    while (terms_cnt < n) {
        idx_t m = next_m++;
        idx_t r = (idx_t)((double)m / E + 0.5);
        if (A054404_small(m) != r) {
            if (terms_cnt == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 64;
                terms_arr = (idx_t *)realloc(terms_arr, sizeof(idx_t) * terms_cap);
            }
            terms_arr[terms_cnt++] = m;
        }
    }
    return (seq_t)terms_arr[n - 1];
}
