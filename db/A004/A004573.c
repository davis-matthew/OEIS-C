/*
 * A004573  Expansion of sqrt(7) in base 6.
 * https://oeis.org/A004573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = n-th base-6 digit of sqrt(7) = floor(sqrt(7*36^(n-1)))
 *            mod 6, computed by the digit-by-digit square-root algorithm in
 *            base 6
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmpn, subn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/cmpn.h"
#include "../utils/subn.h"

static uint32_t *P = NULL, *R = NULL, *T = NULL;
static size_t cap = 0, lp = 0, lr = 0;
static unsigned char *dig = NULL;
static idx_t ndig = 0, digcap = 0;

static void ensure(size_t need)
{
    if (need <= cap) return;
    size_t nc = need * 2 + 16;
    P = (uint32_t *)realloc(P, nc * sizeof(uint32_t));
    R = (uint32_t *)realloc(R, nc * sizeof(uint32_t));
    T = (uint32_t *)realloc(T, nc * sizeof(uint32_t));
    memset(P + cap, 0, (nc - cap) * sizeof(uint32_t));
    memset(R + cap, 0, (nc - cap) * sizeof(uint32_t));
    memset(T + cap, 0, (nc - cap) * sizeof(uint32_t));
    cap = nc;
}

/* d = s*m + add; returns normalized length */
static size_t muladd(uint32_t *d, const uint32_t *s, size_t len, uint32_t m, uint32_t add)
{
    uint64_t c = add;
    for (size_t i = 0; i < len; i++) {
        c += (uint64_t)s[i] * m;
        d[i] = (uint32_t)c;
        c >>= 32;
    }
    while (c) { d[len++] = (uint32_t)c; c >>= 32; }
    while (len && d[len - 1] == 0) len--;
    return len;
}

static void step(void)
{
    size_t m = (lp > lr ? lp : lr) + 4;
    ensure(m);
    uint32_t pairval = (ndig == 0) ? 7 : 0;
    lr = muladd(R, R, lr, 36, pairval);
    uint32_t d = 0;
    size_t lt = 0;
    for (uint32_t t = 5; t >= 1; t--) {
        lt = muladd(T, P, lp, 12, t);
        lt = muladd(T, T, lt, t, 0);
        if (cmpn(T, lt, R, lr) <= 0) { d = t; break; }
    }
    if (d) lr = subn(R, lr, T, lt);
    lp = muladd(P, P, lp, 6, d);

    if (ndig + 2 > digcap) {
        idx_t nc = digcap * 2 + 64;
        dig = (unsigned char *)realloc(dig, (size_t)nc);
        digcap = nc;
    }
    ndig++;
    dig[ndig] = (unsigned char)d;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (ndig < n) step();
    return (seq_t)dig[n];
}
