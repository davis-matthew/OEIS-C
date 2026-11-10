/*
 * A083422  Positions of records in A083403.
 * https://oeis.org/A083422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = position of n-th strict record in A083403, where
 *            A083403(k) = #squarefree in row k of the triangle (= Q(T_k) -
 *            Q(T_{k-1}), Q(x)=Sum_{d>=1} mu(d)*floor(x/d^2)); computed by
 *            scanning rows
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static signed char *rec_mu = 0;
static unsigned long long rec_cap = 0;

static void rec_ensure_mu(unsigned long long limit)
{
    if (limit <= rec_cap && rec_mu) return;
    unsigned long long cap = limit * 2 + 1024;
    free(rec_mu);
    rec_mu = (signed char *)malloc((size_t)cap + 1);
    for (unsigned long long i = 0; i <= cap; i++) rec_mu[i] = 1;
    rec_mu[0] = 0;
    char *comp = (char *)calloc((size_t)cap + 1, 1);
    for (unsigned long long p = 2; p <= cap; p++) {
        if (comp[p]) continue;
        for (unsigned long long m = p; m <= cap; m += p) {
            if (m > p) comp[m] = 1;
            rec_mu[m] = (signed char)(-rec_mu[m]);
        }
        unsigned long long p2 = p * p;
        for (unsigned long long m = p2; m <= cap; m += p2) rec_mu[m] = 0;
    }
    free(comp);
    rec_cap = cap;
}

static long long rec_count_sqfree(unsigned long long x)
{
    long long s = 0;
    unsigned long long d = 1;
    while (d * d <= x) d++;
    d--;
    rec_ensure_mu(d);
    for (unsigned long long k = 1; k <= d; k++) {
        if (rec_mu[k] == 0) continue;
        s += (long long)rec_mu[k] * (long long)(x / (k * k));
    }
    return s;
}

static unsigned long long *rec_pos = 0;
static size_t rec_n = 0, rec_alloc = 0;
static unsigned long long rec_next = 1;
static long long rec_max = 0, rec_prev = 0;

seq_t a(idx_t n)
{
    while (rec_n < (size_t)n) {
        unsigned long long k = rec_next++;
        long long c = rec_count_sqfree(k * (k + 1) / 2);
        long long v = c - rec_prev;
        rec_prev = c;
        if (v > rec_max) {
            rec_max = v;
            if (rec_n == rec_alloc) {
                rec_alloc = rec_alloc ? rec_alloc * 2 : 256;
                rec_pos = (unsigned long long *)realloc(rec_pos, rec_alloc * sizeof(unsigned long long));
            }
            rec_pos[rec_n++] = k;
        }
    }
    return (seq_t)rec_pos[(size_t)n - 1];
}
