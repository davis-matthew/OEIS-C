/*
 * A230799  The number of distinct nonzero coefficients in the n-th cyclotomic
 *          polynomial.
 * https://oeis.org/A230799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct nonzero coefficients in the * n-th
 *            cyclotomic polynomial Φ_n(x)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * The algorithm computes Φ_n(x) by recurrence:
 *   Φ_n(x) = (x^n - 1) / ∏_{d|n, d<n} Φ_d(x).
 * Polynomial multiplication and exact integer division are carried
 * out with small 32‑bit signed coefficients.  The result is cached
 * so that repeated calls are fast.
 */
#include <stdlib.h>
#include <stdbool.h>

static int **poly_cache = NULL;        /* pointer to coefficient arrays */
static size_t *deg_cache = NULL;       /* degree of each poly */
static seq_t *value_cache = NULL;      /* cached answers */
static size_t cache_size = 0;

/* enlarge caches to accommodate n */
static void ensure_cache_size(idx_t n)
{
    if (n < cache_size) return;
    size_t new_size = cache_size > 0 ? cache_size : 16;
    while (n >= new_size) new_size <<= 1;
    poly_cache = (int**)realloc(poly_cache, new_size * sizeof(int*));
    deg_cache = (size_t*)realloc(deg_cache, new_size * sizeof(size_t));
    value_cache = (seq_t*)realloc(value_cache, new_size * sizeof(seq_t));
    for (size_t i = cache_size; i < new_size; ++i) {
        poly_cache[i] = NULL;
        value_cache[i] = 0;
    }
    cache_size = new_size;
}

/* multiply two integer coefficient polynomials a and b */
static int* poly_mul(const int* a, size_t na, const int* b, size_t nb, size_t* outlen)
{
    size_t len = na + nb - 1;
    int* res = (int*)malloc(len * sizeof(int));
    for (size_t i = 0; i < len; ++i) res[i] = 0;
    for (size_t i = 0; i < na; ++i) {
        for (size_t j = 0; j < nb; ++j) {
            long long add = (long long)a[i] * (long long)b[j];
            res[i + j] += (int)add;
        }
    }
    *outlen = len;
    return res;
}

/* compute the cyclotomic polynomial Φ_n(x) and cache it */
static void compute_poly(idx_t n)
{
    if (poly_cache[n] != NULL) return;     /* already computed */
    /* compute all proper divisors first */
    size_t prod_deg = 0;
    int* prod_poly = (int*)malloc(sizeof(int));
    prod_poly[0] = 1;  /* product starts as 1 */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            if (d < n) {
                compute_poly(d);
                size_t tmp_len;
                int* tmp = poly_mul(prod_poly, prod_deg + 1,
                                    poly_cache[d], deg_cache[d] + 1,
                                    &tmp_len);
                free(prod_poly);
                prod_poly = tmp;
                prod_deg = tmp_len - 1;
            }
            idx_t e = n / d;
            if (e != d && e < n) {
                compute_poly(e);
                size_t tmp_len;
                int* tmp = poly_mul(prod_poly, prod_deg + 1,
                                    poly_cache[e], deg_cache[e] + 1,
                                    &tmp_len);
                free(prod_poly);
                prod_poly = tmp;
                prod_deg = tmp_len - 1;
            }
        }
    }

    /* dividend (x^n - 1) */
    size_t div_deg = (size_t)n;
    int* dividend = (int*)calloc(div_deg + 1, sizeof(int));
    dividend[0] = -1;
    dividend[div_deg] = 1;

    /* division dividend / prod_poly → quotient Φ_n */
    size_t q_deg = div_deg - prod_deg;
    int* quotient = (int*)calloc(q_deg + 1, sizeof(int));

    for (int k = (int)div_deg; k >= (int)prod_deg; --k) {
        int coeff = dividend[k];
        quotient[k - (int)prod_deg] = coeff;
        for (size_t j = 0; j <= prod_deg; ++j) {
            long long sub = (long long)coeff * (long long)prod_poly[j];
            dividend[(k - (int)prod_deg) + j] -= (int)sub;
        }
    }

    /* cache results */
    poly_cache[n] = quotient;
    deg_cache[n] = q_deg;

    /* count distinct non‑zero coefficients */
    int uniq[64];
    size_t uniq_cnt = 0;
    for (size_t i = 0; i <= q_deg; ++i) {
        int c = quotient[i];
        if (c == 0) continue;
        bool found = false;
        for (size_t j = 0; j < uniq_cnt; ++j)
            if (uniq[j] == c) { found = true; break; }
        if (!found) uniq[uniq_cnt++] = c;
    }
    value_cache[n] = (seq_t)uniq_cnt;

    free(prod_poly);
    free(dividend);
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */
    ensure_cache_size(n);
    /* we compute for n only when first requested */
    compute_poly(n);
    return value_cache[n];
}
