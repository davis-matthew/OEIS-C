/*
 * A014807  Inverse of 798th cyclotomic polynomial.
 * https://oeis.org/A014807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>=1, a(n) = -sum_{k=1}^{min(216,n)} c_k * a(n-k)
   where c_k are the coefficients of the cyclotomic polynomial Φ_798(x) */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static int cmp_uint64(const void *p1, const void *p2)
{
    idx_t a = *(const idx_t *)p1;
    idx_t b = *(const idx_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

static void poly_mul(const seq_t *a, size_t asz,
                     const seq_t *b, size_t bsz,
                     seq_t **res, size_t *resz)
{
    size_t sz = asz + bsz - 1;
    seq_t *tmp = alloc_seq_array(sz);
    for (size_t i = 0; i < sz; ++i) tmp[i] = 0;
    for (size_t i = 0; i < asz; ++i)
        for (size_t j = 0; j < bsz; ++j)
            tmp[i + j] += a[i] * b[j];
    *res = tmp;
    *resz = sz;
}

static void poly_div_monic(const seq_t *numer, size_t nsz,
                           const seq_t *denom, size_t dsz,
                           seq_t **res, size_t *resz)
{
    seq_t *tmp = alloc_seq_array(nsz);
    for (size_t i = 0; i < nsz; ++i) tmp[i] = numer[i];

    size_t nd = nsz - 1;
    size_t dd = dsz - 1;
    size_t resLen = nd - dd + 1;
    seq_t *q = alloc_seq_array(resLen);

    for (size_t ii = nd + 1; ii-- > 0; ) {
        size_t i = ii;
        seq_t coef = tmp[i];
        if (coef == 0) continue;
        size_t qpos = i - dd;
        q[qpos] = coef;
        for (size_t j = 0; j < dsz; ++j)
            tmp[qpos + j] -= coef * denom[j];
    }
    free_seq_array(tmp);
    *res = q;
    *resz = resLen;
}

static void compute_seq_cache(seq_t *cache)
{
    /* Divisors of 798 = 2*3*7*19 (prime factors) */
    const idx_t primes[4] = {2, 3, 7, 19};
    const size_t nprimes = 4;
    idx_t divs[16];
    size_t divcnt = 0;
    for (int mask = 1; mask < (1 << (int)nprimes); ++mask) {
        idx_t val = 1;
        for (size_t i = 0; i < nprimes; ++i)
            if (mask & (1 << i)) val *= primes[i];
        divs[divcnt++] = val;
    }
    divs[divcnt++] = 1;          /* include 1 */
    qsort(divs, divcnt, sizeof(idx_t), cmp_uint64);

    /* Store Φ_d for each divisor */
    seq_t *phi[16];
    size_t deg[16];

    for (size_t i = 0; i < divcnt; ++i) {
        idx_t d = divs[i];
        if (d == 1) {
            phi[i] = alloc_seq_array(2);
            phi[i][0] = -1;
            phi[i][1] = 1;
            deg[i] = 1;
            continue;
        }

        seq_t *den = alloc_seq_array(1);
        den[0] = 1;
        size_t den_sz = 1;

        for (size_t j = 0; j < i; ++j) {
            if (d % divs[j] == 0) {
                seq_t *tmp = NULL;
                size_t tmp_sz = 0;
                poly_mul(den, den_sz, phi[j], deg[j] + 1, &tmp, &tmp_sz);
                free_seq_array(den);
                den = tmp;
                den_sz = tmp_sz;
            }
        }

        seq_t *num = alloc_seq_array((size_t)d + 1);
        num[0] = -1;
        num[(size_t)d] = 1;

        seq_t *q = NULL;
        size_t qsz = 0;
        poly_div_monic(num, (size_t)d + 1, den, den_sz, &q, &qsz);

        free_seq_array(num);
        free_seq_array(den);

        phi[i] = q;
        deg[i] = qsz - 1;
    }

    /* Locate Φ_798 */
    size_t idx_798 = 0;
    for (size_t i = 0; i < divcnt; ++i)
        if (divs[i] == 798) { idx_798 = i; break; }

    seq_t *phi798 = phi[idx_798];
    size_t deg_phi = deg[idx_798];
    const size_t period = 798;

    cache[0] = 1;
    for (size_t n = 1; n < period; ++n) {
        seq_t sum = 0;
        size_t maxk = (deg_phi <= n) ? deg_phi : n;
        for (size_t k = 1; k <= maxk; ++k)
            sum += phi798[k] * cache[n - k];
        cache[n] = -sum;
    }

    for (size_t i = 0; i < divcnt; ++i)
        free_seq_array(phi[i]);
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static bool initialized = false;

    if (!initialized) {
        cache = alloc_seq_array(798);
        compute_seq_cache(cache);
        initialized = true;
    }

    size_t r = (size_t)(n % 798);
    return cache[r];
}
