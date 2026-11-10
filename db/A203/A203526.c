/*
 * A203526  v(n)/A000178(n); v=A203524 and A000178=(superfactorials).
 * https://oeis.org/A203526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A203524(n)/A000178(n-1), where A203524(n) = Product_{2 <=
 *            i < j <= n+1} (prime(i)+prime(j)) and A000178(n-1) =
 *            Product_{m=1..n-1} m^(n-m); computed via prime-exponent
 *            bookkeeping
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void A203526_add_factors(const uint32_t *spf, long long *ex, uint64_t m, long long mult)
{
    while (m > 1) {
        uint32_t p = spf[m];
        ex[p] += mult;
        m /= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t cnt = (size_t)n + 1;

    /* find first n+1 primes */
    size_t lim = 32;
    uint8_t *comp = NULL;
    for (;;) {
        comp = (uint8_t *)calloc(lim + 1, 1);
        if (!comp) abort();
        size_t c = 0;
        for (size_t i = 2; i <= lim; i++) {
            if (!comp[i]) {
                c++;
                for (size_t j = i * i; j <= lim; j += i) comp[j] = 1;
            }
        }
        if (c >= cnt) break;
        free(comp);
        lim *= 2;
    }
    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!pr) abort();
    size_t c = 0;
    for (size_t i = 2; i <= lim && c < cnt; i++)
        if (!comp[i]) pr[c++] = i;
    free(comp);

    /* sieve of smallest prime factors up to M */
    uint64_t M = pr[cnt - 1] + pr[cnt - 2];
    if (M < (uint64_t)n) M = (uint64_t)n;
    uint32_t *spf = (uint32_t *)calloc((size_t)M + 1, sizeof(uint32_t));
    long long *ex = (long long *)calloc((size_t)M + 1, sizeof(long long));
    if (!spf || !ex) abort();
    for (uint64_t i = 2; i <= M; i++) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= M; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }

    /* numerator: pairs of primes prime(2..n+1), 0-based indices 1..n */
    for (size_t i = 1; i <= (size_t)n; i++)
        for (size_t j = i + 1; j <= (size_t)n; j++)
            A203526_add_factors(spf, ex, pr[i] + pr[j], 1);

    /* denominator: A000178(n-1) = Product_{m=1..n-1} m^(n-m) */
    for (uint64_t m = 2; m < (uint64_t)n; m++)
        A203526_add_factors(spf, ex, m, -(long long)((uint64_t)n - m));

    seq_t res = 1;
    for (uint64_t p = 2; p <= M; p++) {
        if (spf[p] == p && ex[p] > 0) {
            for (long long k = 0; k < ex[p]; k++) res *= (seq_t)p;
        }
    }
    free(pr);
    free(spf);
    free(ex);
    return res;
}
