/*
 * A065372  a(n) = A065371(A065371(n)).
 * https://oeis.org/A065372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = A065371(A065371(n)), where A065371 is multiplicative with
 *            A065371(prime(k)) = prime(k) - k
 * Range:     u8 n <= 432, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 9475 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A065372_pi = NULL;
static uint64_t A065372_lim = 0;

static void A065372_ensure(uint64_t lim)
{
    if (A065372_lim >= lim && A065372_pi) return;
    if (lim < 2) lim = 2;
    unsigned char *s = (unsigned char *)malloc((size_t)lim + 1);
    if (!s) abort();
    memset(s, 1, (size_t)lim + 1);
    s[0] = 0; s[1] = 0;
    for (uint64_t i = 2; i * i <= lim; ++i)
        if (s[i])
            for (uint64_t j = i * i; j <= lim; j += i) s[j] = 0;
    uint32_t *t = (uint32_t *)realloc(A065372_pi, ((size_t)lim + 1) * sizeof(uint32_t));
    if (!t) abort();
    uint32_t c = 0;
    for (uint64_t i = 0; i <= lim; ++i) {
        if (s[i]) ++c;
        t[i] = c;
    }
    free(s);
    A065372_pi = t;
    A065372_lim = lim;
}

static uint64_t A065372_f(uint64_t m)
{
    uint64_t res = 1;
    if (m <= 1) return 1;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t base = p - A065372_pi[p];
            while (m % p == 0) {
                m /= p;
                res *= base;
            }
        }
    }
    if (m > 1) res *= (m - A065372_pi[m]);
    return res;
}

seq_t a(idx_t n)
{
    A065372_ensure((uint64_t)n);
    uint64_t inner = A065372_f((uint64_t)n);
    uint64_t out = A065372_f(inner);
    return (seq_t)out;
}
