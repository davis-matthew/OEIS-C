/*
 * A007686  Prime(n)*...*a(n) is the least product of consecutive primes which
 *          is non-deficient.
 * https://oeis.org/A007686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) is the least prime q >= prime(n) such that
 *            Product_{prime(n) <= p <= q} (p+1)/p >= 2, i.e. the product of
 *            the consecutive primes prime(n)*...*q is non-deficient. The
 *            primes come from a sieve, and the test compares prefix sums of
 *            log(1+1/p) with log(2)
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

static uint32_t *pr = NULL;     /* primes, pr[0] = 2 */
static long double *ps = NULL;  /* ps[k] = sum_{i<k} log(1+1/pr[i]) */
static uint64_t npr = 0;
static uint64_t lim = 0;

static void build(uint64_t L) {
    uint64_t half = L / 2 + 1, i, j, cnt = 0;
    unsigned char *s = (unsigned char *)malloc((size_t)half);
    memset(s, 1, (size_t)half);
    s[0] = 0; /* 1 is not prime */
    for (i = 1; (2 * i + 1) * (2 * i + 1) <= L; i++) {
        if (s[i]) {
            uint64_t p = 2 * i + 1;
            for (j = p * p / 2; j < half; j += p) s[j] = 0;
        }
    }
    for (i = 1; i < half; i++) if (s[i] && 2 * i + 1 <= L) cnt++;
    cnt += 1;
    free(pr); free(ps);
    pr = (uint32_t *)malloc((size_t)cnt * sizeof(uint32_t));
    ps = (long double *)malloc((size_t)(cnt + 1) * sizeof(long double));
    pr[0] = 2;
    j = 1;
    for (i = 1; i < half; i++) if (s[i] && 2 * i + 1 <= L) pr[j++] = (uint32_t)(2 * i + 1);
    free(s);
    npr = cnt;
    ps[0] = 0.0L;
    for (i = 0; i < cnt; i++) ps[i + 1] = ps[i] + log1pl(1.0L / (long double)pr[i]);
    lim = L;
}

seq_t a(idx_t n) {
    const long double LN2 = 0.693147180559945309417232121458176568L;
    const long double EPS = 1e-15L; /* exact equality only for 2*3 = 6 (perfect) */
    uint64_t k = (uint64_t)n;
    uint64_t L;
    uint64_t lo, hi;
    if (lim == 0) build(1u << 16);
    for (;;) {
        if (k + 1 <= npr && ps[npr] - ps[k - 1] >= LN2 - EPS) break;
        L = lim * 4;
        if (L > 4000000000ULL) L = 4000000000ULL;
        if (L == lim) break;
        build(L);
    }
    /* smallest m (count of primes) with ps[m] - ps[k-1] >= LN2 - EPS, m >= k */
    lo = k; hi = npr;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (ps[mid] - ps[k - 1] >= LN2 - EPS) hi = mid;
        else lo = mid + 1;
    }
    return (seq_t)pr[lo - 1];
}
