/*
 * A086749  Partial sums of A038580.
 * https://oeis.org/A086749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} A038580(k), where A038580(k) =
 *            prime(prime(prime(k)))
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 2637, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 9365 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mem.h"

static uint64_t *ps_primes = NULL;
static size_t ps_cnt = 0;

static void ps_rebuild(size_t limit)
{
    uint8_t *comp = (uint8_t *)calloc(limit + 1, 1);
    if (!comp) abort();
    for (size_t i = 2; i * i <= limit; ++i)
        if (!comp[i])
            for (size_t j = i * i; j <= limit; j += i) comp[j] = 1;
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (!comp[i]) ++cnt;
    uint64_t *np = (uint64_t *)malloc((cnt ? cnt : 1) * sizeof(uint64_t));
    if (!np) abort();
    size_t k = 0;
    for (size_t i = 2; i <= limit; ++i) if (!comp[i]) np[k++] = i;
    free(comp);
    free(ps_primes);
    ps_primes = np;
    ps_cnt = cnt;
}

static void ps_ensure(uint64_t need)
{
    if (ps_cnt >= (size_t)need) return;
    size_t limit;
    if (need < 6) limit = 15;
    else {
        double x = (double)need;
        limit = (size_t)(x * (log(x) + log(log(x)))) + 10;
    }
    ps_rebuild(limit);
    while (ps_cnt < (size_t)need) {
        limit *= 2;
        ps_rebuild(limit);
    }
}

static seq_t *ps_sums = NULL;   /* ps_sums[k] = a(k), index 0 -> 0 */
static size_t ps_sums_cap = 0;
static size_t ps_done = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t N = (size_t)n;
    if (N > ps_done) {
        if (N + 1 > ps_sums_cap) {
            size_t nc = N + 1;
            if (ps_sums == NULL) ps_sums = mem_new(seq_t, nc);
            else ps_sums = mem_resize(seq_t, ps_sums, nc);
            ps_sums_cap = nc;
        }
        ps_ensure(N);
        uint64_t p1max = ps_primes[N - 1];
        ps_ensure(p1max);
        uint64_t p2max = ps_primes[p1max - 1];
        ps_ensure(p2max);
        seq_t acc = (seq_t)0;
        if (ps_done > 0) acc = ps_sums[ps_done];
        else ps_sums[0] = (seq_t)0;
        for (size_t k = ps_done + 1; k <= N; ++k) {
            uint64_t p1 = ps_primes[k - 1];
            uint64_t p2 = ps_primes[p1 - 1];
            uint64_t p3 = ps_primes[p2 - 1];
            acc += (seq_t)p3;
            ps_sums[k] = acc;
        }
        ps_done = N;
    }
    return ps_sums[N];
}
