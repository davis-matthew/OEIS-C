/*
 * A077031  Numbers k such that A068340(k)=+/-3.
 * https://oeis.org/A077031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = n-th k with A068340(k) = +/-3, where A068340(k) =
 *            Sum_{j=1..k} mu(j)*j; computed by a sieve search (no further
 *            terms are known below 10^12)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A077031_LIMIT 4000000

static bool a077031_done = false;
static size_t a077031_cnt = 0;
static uint64_t a077031_terms[64];

static void a077031_init(void)
{
    if (a077031_done) return;
    a077031_done = true;
    const long N = A077031_LIMIT;
    int8_t *mu = (int8_t *)malloc((size_t)N + 1);
    uint8_t *comp = (uint8_t *)calloc((size_t)N + 1, 1);
    if (!mu || !comp) { free(mu); free(comp); return; }
    for (long i = 0; i <= N; ++i) mu[i] = 1;
    for (long i = 2; i <= N; ++i) {
        if (!comp[i]) {
            for (long j = i; j <= N; j += i) {
                if (j != i) comp[j] = 1;
                mu[j] = (int8_t)(-mu[j]);
            }
            if (i <= N / i) {
                long sq = i * i;
                for (long j = sq; j <= N; j += sq) mu[j] = 0;
            }
        }
    }
    long long s = 0;
    for (long k = 1; k <= N; ++k) {
        s += (long long)mu[k] * k;
        if ((s == 3 || s == -3) && a077031_cnt < 64)
            a077031_terms[a077031_cnt++] = (uint64_t)k;
    }
    free(mu);
    free(comp);
}

seq_t a(idx_t n)
{
    a077031_init();
    if (n == 0 || n > a077031_cnt) return (seq_t)0;
    return (seq_t)a077031_terms[n - 1];
}
