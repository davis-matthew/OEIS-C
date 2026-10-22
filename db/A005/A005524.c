/*
 * A005524  Values k arising from a construction of Hirschfeld of k-arcs on
 *          elliptic curves over GF(q), where q = A246655(n) is the n-th prime
 *          power > 1.
 * https://oeis.org/A005524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(A005523(n)/2), where for q = A246655(n) = p^e, t =
 *            floor(2*sqrt(q)), A005523(n) = q + t if e is odd, e >= 3 and p |
 *            t, otherwise q + 1 + t (Waterhouse). Prime powers found by a
 *            sieve
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a5524_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a5524_isqrt.h"

static uint64_t *pp_list = NULL;   /* prime powers > 1, in order */
static uint64_t *pp_base = NULL;   /* prime p */
static uint64_t *pp_exp = NULL;    /* exponent e */
static uint64_t pp_count = 0;
static uint64_t pp_limit = 0;

static void a5524_build(uint64_t limit) {
    uint32_t *spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    uint64_t i, j, cnt = 0;
    for (i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 2; i <= limit; i++) {
        uint64_t p = spf[i], m = i, e = 0;
        while (m % p == 0) { m /= p; e++; }
        if (m == 1) cnt++;
    }
    free(pp_list); free(pp_base); free(pp_exp);
    pp_list = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    pp_base = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    pp_exp = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (i = 2; i <= limit; i++) {
        uint64_t p = spf[i], m = i, e = 0;
        while (m % p == 0) { m /= p; e++; }
        if (m == 1) {
            pp_list[cnt] = i;
            pp_base[cnt] = p;
            pp_exp[cnt] = e;
            cnt++;
        }
    }
    free(spf);
    pp_count = cnt;
    pp_limit = limit;
}

seq_t a(idx_t n) {
    uint64_t lim = pp_limit ? pp_limit : 1024;
    while (pp_count < n) {
        if (pp_limit) lim *= 2;
        a5524_build(lim);
    }
    uint64_t q = pp_list[n - 1];
    uint64_t p = pp_base[n - 1];
    uint64_t e = pp_exp[n - 1];
    uint64_t t = a5524_isqrt(4 * q);
    uint64_t N;
    if (e >= 3 && (e & 1) && t % p == 0) N = q + t;
    else N = q + 1 + t;
    return (seq_t)(N / 2);
}
