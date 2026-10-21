/*
 * A063670  Positions of nonzero coefficients in cyclotomic polynomial
 *          Phi_n(x), converted from binary to decimal.
 * https://oeis.org/A063670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the integer whose binary expansion has a 1 in the i‑th
 *            position iff the coefficient of x^i in the cyclotomic polynomial
 *            Φ_n(x) is non‑zero (positions are counted from 0). That binary
 *            number is then interpreted in base 10
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

typedef struct { int32_t *c; idx_t deg; } poly_t;

/* Multiply a monic polynomial by (x^k - 1). */
static poly_t poly_mul_xk_minus_1(const poly_t *p, idx_t k)
{
    idx_t new_deg = p->deg + k;
    int32_t *newc = (int32_t*)calloc(new_deg + 1, sizeof(int32_t));
    for (idx_t i = 0; i <= p->deg; ++i) {
        int32_t v = p->c[i];
        newc[i] -= v;          /* from the "-1" part */
        newc[i + k] += v;      /* from the "x^k" part */
    }
    return (poly_t){ newc, new_deg };
}

/* Divide a monic polynomial by (x^k - 1), exact division guaranteed
   by the cyclotomic construction. */
static poly_t poly_div_xk_minus_1(const poly_t *p, idx_t k)
{
    idx_t new_deg = p->deg - k;
    int32_t *newc = (int32_t*)calloc(new_deg + 1, sizeof(int32_t));
    /* Solve (x^k - 1)*q = p for q; recurrence:
       q[j] = -p[j] + (j>=k ? q[j-k] : 0)  for j = 0 .. new_deg   */
    for (idx_t j = 0; j <= new_deg; ++j) {
        int32_t val = -p->c[j];
        if (j >= k) val += newc[j - k];
        newc[j] = val;
    }
    return (poly_t){ newc, new_deg };
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;   /* convention from the OEIS entry */

    /* ----------------------------------------------------------------
       Factor n into distinct primes (we only need distinct primes for
       generating square‑free divisors).    */
    idx_t tmp = n;
    /* primes up to 100 – enough for all n <= 10000                            */
    const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,
                                     41,43,47,53,59,61,67,71,73,79,83,89,97};
    const size_t np = sizeof(small_primes)/sizeof(small_primes[0]);

    idx_t primes[10];
    size_t primecnt = 0;
    for (size_t i = 0; i < np && small_primes[i]*small_primes[i] <= tmp; ++i) {
        if (tmp % small_primes[i] == 0) {
            primes[primecnt++] = small_primes[i];
            while (tmp % small_primes[i] == 0)
                tmp /= small_primes[i];
        }
    }
    if (tmp > 1) primes[primecnt++] = tmp;

    /* ----------------------------------------------------------------
       Generate all square‑free divisors d and their Möbius value μ(d).   */
    size_t maxD = 1u << primecnt;
    int32_t *divs = (int32_t*)malloc(maxD * sizeof(int32_t));
    int32_t *mus  = (int32_t*)malloc(maxD * sizeof(int32_t));
    size_t cnt = 1;
    divs[0] = 1; mus[0] = 1;
    for (size_t pi = 0; pi < primecnt; ++pi) {
        size_t old = cnt;
        for (size_t j = 0; j < old; ++j) {
            divs[cnt] = divs[j] * primes[pi];
            mus[cnt]  = -mus[j];
            ++cnt;
        }
    }

    /* Separate multipliers (μ = +1) and divisors (μ = -1). */
    idx_t *mul_k = (idx_t*)malloc(cnt * sizeof(idx_t));
    idx_t *div_k = (idx_t*)malloc(cnt * sizeof(idx_t));
    size_t mulcnt = 0, divcnt = 0;
    for (size_t j = 0; j < cnt; ++j) {
        idx_t k = n / (idx_t)divs[j];          /* n/d */
        if (mus[j] == 1) mul_k[mulcnt++] = k;
        else            div_k[divcnt++] = k;
    }
    free(divs); free(mus);

    /* ----------------------------------------------------------------
       Build φ_n(x) as a polynomial with integer coefficients.         */
    poly_t poly;
    poly.deg = 0;
    poly.c   = (int32_t*)calloc(1, sizeof(int32_t));
    poly.c[0] = 1;                         /* start with 1 */

    for (size_t i = 0; i < mulcnt; ++i) {
        poly_t tmp = poly_mul_xk_minus_1(&poly, mul_k[i]);
        free(poly.c);
        poly = tmp;
    }
    for (size_t i = 0; i < divcnt; ++i) {
        poly_t tmp = poly_div_xk_minus_1(&poly, div_k[i]);
        free(poly.c);
        poly = tmp;
    }
    free(mul_k); free(div_k);

    /* ----------------------------------------------------------------
       Sum 2^i for all positions i where coefficient is non‑zero.      */
    seq_t res = 0;
    for (idx_t i = 0; i <= poly.deg; ++i) {
        if (poly.c[i] != 0)
            res += pow2(i);
    }
    free(poly.c);
    return res;
}
