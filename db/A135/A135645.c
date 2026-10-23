/*
 * A135645  Expansion of 1/p(1/x) where p(x) = (-17 - 98*x^9 + x^10) * (-1 -
 *          2*x^9 + x^10)^3 * (-1 - x^9 + x^10)^6.
 * https://oeis.org/A135645
 *
 * Offset:    100    (a(100) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 102, u32 n <= 104, u64 n <= 109,
 *            u128 n <= 115, big any n
 * Helpers:   mul_small, seq_alloc_array, seq_one, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = coefficient of x^n in 1/((-17-98*x^9+x^10)(-1-2*x^9+x^10)^3(-1-x^9+x^10)^6) with offset 100.
   Recurrence (n≥101): a(n) = - Σ_{i=1}^{100} r[i] * a(n-i),
   where r[0..100] are the coefficients of the reversed polynomial of p(x). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_one.h"
#include "../utils/mul_small.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_realloc.h"

#define ORDER 100

static int64_t rec[ORDER + 1];
static seq_t *terms = NULL;
static idx_t terms_cap = 0;
static idx_t computed_max = 0;

static void poly_mul(const int64_t *a, int deg_a,
                     const int64_t *b, int deg_b,
                     int64_t *res, int *deg_res)
{
    int d = deg_a + deg_b;
    for (int i = 0; i <= d; ++i) res[i] = 0;
    for (int i = 0; i <= deg_a; ++i) if (a[i] != 0)
        for (int j = 0; j <= deg_b; ++j) if (b[j] != 0)
            res[i + j] += a[i] * b[j];
    *deg_res = d;
}

static void init_coeffs(void)
{
    /* coefficients of the factors */
    int64_t factor1[11] = {0};
    factor1[0]  = -17;
    factor1[9]  = -98;
    factor1[10] = 1;

    int64_t factor2[11] = {0};
    factor2[0] = -1;
    factor2[9] = -2;
    factor2[10] = 1;

    int64_t factor3[11] = {0};
    factor3[0] = -1;
    factor3[9] = -1;
    factor3[10] = 1;

    /* compute (factor2)^3 */
    int64_t temp1[101], temp2[101], temp3[101];
    int deg_temp1, deg_temp2, deg_temp3;
    poly_mul(factor2, 10, factor2, 10, temp1, &deg_temp1);
    poly_mul(temp1, deg_temp1, factor2, 10, temp2, &deg_temp2);
    int64_t f2pow[101];
    for (int i = 0; i <= deg_temp2; ++i) f2pow[i] = temp2[i];
    int f2deg = deg_temp2;  /* should be 30 */

    /* compute (factor3)^6 */
    int64_t cur[101], next[101];
    for (int i = 0; i <= 10; ++i) cur[i] = factor3[i];
    int cur_deg = 10;
    for (int t = 1; t < 6; ++t) {
        poly_mul(cur, cur_deg, factor3, 10, next, &deg_temp3);
        for (int i = 0; i <= deg_temp3; ++i) cur[i] = next[i];
        cur_deg = deg_temp3;
    }
    int64_t f3pow[101];
    for (int i = 0; i <= cur_deg; ++i) f3pow[i] = cur[i];
    int f3deg = cur_deg;  /* should be 60 */

    /* final product: factor1 * f2pow * f3pow */
    int64_t tmp[101], final[101];
    int deg_tmp, deg_final;
    poly_mul(factor1, 10, f2pow, f2deg, tmp, &deg_tmp);
    poly_mul(tmp, deg_tmp, f3pow, f3deg, final, &deg_final);

    /* reverse to get coefficients of q(x) */
    rec[0] = final[100];
    for (int k = 1; k <= ORDER; ++k) {
        rec[k] = final[100 - k];
    }
}

static void init_terms(void)
{
    if (terms != NULL) return;
    terms_cap = 20001;  /* sufficient for n up to about 20000 */
    terms = seq_alloc_array(terms_cap);
    for (idx_t i = 0; i < terms_cap; ++i) terms[i] = 0;
    terms[100] = seq_one();
    computed_max = 100;
}

static void ensure_terms(idx_t n)
{
    if (n <= computed_max) return;

    if (n + 1 > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        terms = seq_realloc(terms, terms_cap, new_cap);
        for (idx_t i = terms_cap; i <= n; ++i) terms[i] = 0;
        terms_cap = new_cap;
    }

    for (idx_t k = computed_max + 1; k <= n; ++k) {
        if (k < 100) {
            terms[k] = 0;
            continue;
        }
        seq_t pos = 0;
        seq_t neg = 0;
        for (int i = 1; i <= ORDER; ++i) {
            idx_t idx = k - i;
            if (idx > computed_max) continue;
            seq_t val = terms[idx];
            if (val == 0) continue;
            if (rec[i] < 0) {
                pos += mul_small(val, (unsigned int)(-rec[i]));
            } else {
                neg += mul_small(val, (unsigned int)rec[i]);
            }
        }
        terms[k] = pos - neg;
    }
    computed_max = n;
}

seq_t a(idx_t n)
{
    if (n < 100) return (seq_t)0;
    static bool initialized = false;
    if (!initialized) {
        init_coeffs();
        init_terms();
        initialized = true;
    }
    ensure_terms(n);
    return terms[n];
}
