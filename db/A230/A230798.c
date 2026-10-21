/*
 * A230798  The number of distinct coefficients in the n-th cyclotomic
 *          polynomial.
 * https://oeis.org/A230798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct coefficients in the n-th cyclotomic
 *            polynomial Φ_n(x)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   poly_div_xk_minus1, poly_mul_xk_minus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation directly builds Φ_n(x) using the product of (x^(n/d)-1)^μ(d)   */
/* then counts distinct coefficient values. It works for any seq_t and idx_t type.   */
/* The algorithm is O(n·φ(n)) per call, fast enough for n up to about 10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/poly_div_xk_minus1.h"
#include "../utils/poly_mul_xk_minus1.h"

/* comparison function for qsort */
static int cmp_int(const void *a, const void *b) {
    int32_t ia = *(const int32_t *)a;
    int32_t ib = *(const int32_t *)b;
    return (ia < ib) - (ia > ib);
}

/* structure to hold prime factor and exponent */
struct Fact { uint64_t p; int exp; };

/* factorize n into primes; fac_arr must be large enough (e.g. 20) */
static void factorize(uint64_t n, struct Fact *fac_arr, size_t *cnt) {
    size_t idx = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            fac_arr[idx].p = p;
            fac_arr[idx].exp = e;
            ++idx;
        }
    }
    if (n > 1) {
        fac_arr[idx].p = n;
        fac_arr[idx].exp = 1;
        ++idx;
    }
    *cnt = idx;
}

/* structure to hold a divisor and its Möbius value */
struct Div { uint64_t d; int mu; };

/* recursive generation of all divisors and μ(d) */
static void gen_divisors_rec(size_t i, uint64_t cur, bool squarefree, int used,
                             const struct Fact *fac, size_t fac_cnt,
                             struct Div *list, size_t *len) {
    if (i == fac_cnt) {
        int mu = 0;
        if (squarefree) mu = (used % 2 == 0) ? 1 : -1;
        list[(*len)].d = cur;
        list[(*len)].mu = mu;
        ++(*len);
        return;
    }
    /* exponent 0 */
    gen_divisors_rec(i + 1, cur, squarefree, used, fac, fac_cnt, list, len);
    uint64_t cur2 = cur;
    for (int e = 1; e <= fac[i].exp; ++e) {
        cur2 *= fac[i].p;
        bool new_sf = squarefree && (e == 1);
        int new_used = used + (e == 1 ? 1 : 0);
        gen_divisors_rec(i + 1, cur2, new_sf, new_used, fac, fac_cnt, list, len);
    }
}

/* count number of distinct integers in array of length len */
static size_t count_distinct(const int32_t *arr, size_t len) {
    if (len == 0) return 0;
    int32_t *tmp = (int32_t *)malloc(len * sizeof(int32_t));
    memcpy(tmp, arr, len * sizeof(int32_t));
    qsort(tmp, len, sizeof(int32_t), cmp_int);
    size_t distinct = 1;
    for (size_t i = 1; i < len; ++i) {
        if (tmp[i] != tmp[i - 1]) ++distinct;
    }
    free(tmp);
    return distinct;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset >0, but guard */
    /* factorization */
    struct Fact fac_arr[64];
    size_t fac_cnt;
    factorize((uint64_t)n, fac_arr, &fac_cnt);
    /* estimate divisors count */
    size_t div_est = 1;
    for (size_t i = 0; i < fac_cnt; ++i) div_est *= (fac_arr[i].exp + 1);
    struct Div *divs = (struct Div *)malloc(div_est * sizeof(struct Div));
    size_t div_len = 0;
    gen_divisors_rec(0, 1, true, 0, fac_arr, fac_cnt, divs, &div_len);

    /* build cyclotomic polynomial */
    int32_t *poly = (int32_t *)malloc(sizeof(int32_t));
    poly[0] = 1;
    size_t deg = 0;

    /* multiply parts with μ = +1 */
    for (size_t i = 0; i < div_len; ++i) {
        if (divs[i].mu == 1) {
            size_t k = (size_t)(n / divs[i].d);
            poly_mul_xk_minus1(&poly, &deg, k);
        }
    }
    /* divide parts with μ = -1 */
    for (size_t i = 0; i < div_len; ++i) {
        if (divs[i].mu == -1) {
            size_t k = (size_t)(n / divs[i].d);
            poly_div_xk_minus1(&poly, &deg, k);
        }
    }
    free(divs);

    /* count distinct coefficients */
    size_t distinct = count_distinct(poly, deg + 1);
    free(poly);

    return (seq_t)distinct;
}
