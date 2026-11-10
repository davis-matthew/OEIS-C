/*
 * A091315  Number of orbits of length n under the map whose periodic points
 *          are counted by A061684.
 * https://oeis.org/A091315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*Sum_{d|n} mu(d)*b(n/d), where b(m) = A061684(m)
 *            with A061684(m) = (1/m)*Sum_{k=0..m-1}
 *            binomial(m,k)^3*(m-k)*A061684(k), A061684(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A091315_cache = NULL;
static size_t A091315_cnt = 0;   /* terms 0..cnt-1 computed */

static void A091315_fill(size_t n)
{
    if (n < A091315_cnt) return;
    seq_t *tmp = mem_resize(seq_t, A091315_cache, n + 1);
    if (!tmp) abort();
    A091315_cache = tmp;
    if (A091315_cnt == 0) {
        A091315_cache[0] = 1;
        A091315_cnt = 1;
    }
    for (size_t m = A091315_cnt; m <= n; ++m) {
        seq_t sum = 0;
        seq_t bin = 1;
        for (size_t k = 0; k < m; ++k) {
            seq_t term = A091315_cache[k];
            term *= (seq_t)(m - k);
            term *= bin;
            term *= bin;
            term *= bin;
            sum += term;
            if (k + 1 < m) {
                bin *= (seq_t)(m - k);
                bin /= (seq_t)(k + 1);
            }
        }
        A091315_cache[m] = sum / (seq_t)m;
    }
    A091315_cnt = n + 1;
}

/* mobius of small integer */
static int A091315_mu(unsigned long long d)
{
    int r = 1;
    for (unsigned long long p = 2; p * p <= d; ++p) {
        if (d % p == 0) {
            d /= p;
            if (d % p == 0) return 0;
            r = -r;
        }
    }
    if (d > 1) r = -r;
    return r;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    A091315_fill(N);
    seq_t pos = 0, neg = 0;
    for (size_t d = 1; d <= N; ++d) {
        if (N % d != 0) continue;
        int mu = A091315_mu(d);
        if (mu > 0) pos += A091315_cache[N / d];
        else if (mu < 0) neg += A091315_cache[N / d];
    }
    return (pos - neg) / (seq_t)N;
}
