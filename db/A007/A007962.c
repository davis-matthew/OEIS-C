/*
 * A007962  a(n) is the largest odd number k such that 9, 11, ..., k are sums
 *          of 3 of first n odd primes.
 * https://oeis.org/A007962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: mark all sums of two of the first n odd primes, then the
 *            answer is the largest odd k such that every odd m with 9 <= m <=
 *            k equals p + s for some prime p and pair sum s
 * Range:     u8 n <= 23, u16 n <= 2451, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static long long *a007962_primes = NULL;
static long long a007962_np = 0;

static void a007962_ensure(long long n) {
    long long lim = 64;
    if (a007962_np >= n) return;
    for (;;) {
        unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        long long cnt = 0, i, j;
        for (i = 2; i * i <= lim; i++)
            if (!comp[i])
                for (j = i * i; j <= lim; j += i) comp[j] = 1;
        for (i = 3; i <= lim; i += 2)
            if (!comp[i]) cnt++;
        if (cnt >= n) {
            free(a007962_primes);
            a007962_primes = (long long *)malloc((size_t)cnt * sizeof(long long));
            a007962_np = 0;
            for (i = 3; i <= lim; i += 2)
                if (!comp[i]) a007962_primes[a007962_np++] = i;
            free(comp);
            return;
        }
        free(comp);
        lim *= 2;
    }
}

seq_t a(idx_t n) {
    long long N = (long long)n, i, j, k, best;
    long long *p;
    long long pmax, lim2;
    unsigned char *s2;

    a007962_ensure(N);
    p = a007962_primes;
    pmax = p[N - 1];
    lim2 = 2 * pmax;

    s2 = (unsigned char *)calloc((size_t)lim2 + 1, 1);
    for (i = 0; i < N; i++)
        for (j = i; j < N; j++)
            s2[p[i] + p[j]] = 1;

    best = 7;
    for (k = 9; k <= 3 * pmax; k += 2) {
        bool ok = false;
        for (i = 0; i < N && p[i] <= k - 6; i++) {
            long long r = k - p[i];
            if (r <= lim2 && s2[r]) { ok = true; break; }
        }
        if (!ok) break;
        best = k;
    }
    free(s2);
    return (seq_t)best;
}
