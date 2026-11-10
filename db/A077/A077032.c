/*
 * A077032  Numbers k such that A068340(k)=+/-4.
 * https://oeis.org/A077032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = n-th k such that |Sum_{j=1..k} mu(j)*j| = 4
 *            (A068340(k)=+/-4); computed by sieve of mu up to a limit (known
 *            further terms exceed 10^12)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A077032_LIMIT 20000

static int a077032_ready = 0;
static int a077032_cnt = 0;
static long long a077032_terms[64];

static void a077032_init(void)
{
    if (a077032_ready) return;
    a077032_ready = 1;
    int N = A077032_LIMIT;
    signed char *mu = (signed char *)malloc(N + 1);
    unsigned char *comp = (unsigned char *)malloc(N + 1);
    if (!mu || !comp) exit(1);
    memset(comp, 0, N + 1);
    for (int i = 0; i <= N; i++) mu[i] = 1;
    for (int i = 2; i <= N; i++) {
        if (!comp[i]) {
            for (int j = i; j <= N; j += i) {
                if (j > i) comp[j] = 1;
                mu[j] = (signed char)(-mu[j]);
            }
            long long sq = (long long)i * i;
            for (long long j = sq; j <= N; j += sq) mu[j] = 0;
        }
    }
    long long s = 0;
    for (int k = 1; k <= N; k++) {
        s += (long long)mu[k] * k;
        if ((s == 4 || s == -4) && a077032_cnt < 64)
            a077032_terms[a077032_cnt++] = k;
    }
    free(mu);
    free(comp);
}

seq_t a(idx_t n)
{
    a077032_init();
    if (n == 0 || n > (idx_t)a077032_cnt) return (seq_t)0;
    return (seq_t)a077032_terms[n - 1];
}
