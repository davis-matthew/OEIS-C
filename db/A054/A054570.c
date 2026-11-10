/*
 * A054570  Prime number spiral (clockwise, West spoke).
 * https://oeis.org/A054570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000040(A033951(n)) = prime(4*n^2 + 3*n + 1); computed by
 *            an odd-only sieve of Eratosthenes
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 763 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static unsigned char *a054570_sv = NULL;
static uint64_t a054570_lim = 0; /* sieve covers odd numbers 2i+1 <= a054570_lim */

static void a054570_build(uint64_t lim)
{
    uint64_t half = lim / 2 + 1; /* indices 0..half-1 -> 2i+1 */
    uint64_t i, j;
    unsigned char *s = (unsigned char *)calloc((size_t)half, 1);
    if (!s)
        return;
    for (i = 1; (2 * i + 1) * (2 * i + 1) <= lim; i++) {
        if (!s[i]) {
            uint64_t p = 2 * i + 1;
            for (j = (p * p - 1) / 2; j < half; j += p)
                s[j] = 1;
        }
    }
    free(a054570_sv);
    a054570_sv = s;
    a054570_lim = lim;
}

seq_t a(idx_t n)
{
    uint64_t m = 4 * (uint64_t)n * (uint64_t)n + 3 * (uint64_t)n + 1;
    uint64_t lim, i, cnt, half;
    if (m == 1)
        return (seq_t)2;
    if (m < 6) {
        lim = 15;
    } else {
        double d = (double)m;
        lim = (uint64_t)(d * (log(d) + log(log(d)))) + 10;
    }
    if (!a054570_sv || a054570_lim < lim)
        a054570_build(lim);
    if (!a054570_sv)
        return (seq_t)0;
    half = a054570_lim / 2 + 1;
    cnt = 1; /* the prime 2 */
    for (i = 1; i < half; i++) {
        if (!a054570_sv[i]) {
            cnt++;
            if (cnt == m)
                return (seq_t)(2 * i + 1);
        }
    }
    return (seq_t)0;
}
