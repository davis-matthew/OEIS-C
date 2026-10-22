/*
 * A000059  Numbers k such that (2k)^4 + 1 is prime.
 * https://oeis.org/A000059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000068(n+1)/2; search over k: test whether (2k)^4 + 1 is
 *            prime, using trial division by small primes p == 1 (mod 8), then
 *            deterministic Miller-Rabin with bases 2..41 in 128-bit
 *            Montgomery arithmetic
 * Range:     u8 n <= 63, u16 n <= 8141, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 u128_t;

static void mul128(u128_t a, u128_t b, u128_t *hi, u128_t *lo)
{
    const u128_t M64 = (u128_t)0xFFFFFFFFFFFFFFFFULL;
    u128_t a0 = a & M64, a1 = a >> 64, b0 = b & M64, b1 = b >> 64;
    u128_t p00 = a0 * b0, p01 = a0 * b1, p10 = a1 * b0, p11 = a1 * b1;
    u128_t mid = (p00 >> 64) + (p01 & M64) + (p10 & M64);
    *lo = (p00 & M64) | ((mid & M64) << 64);
    *hi = p11 + (p01 >> 64) + (p10 >> 64) + (mid >> 64);
}

static u128_t redc(u128_t hi, u128_t lo, u128_t N, u128_t np)
{
    u128_t m = lo * np, mh, ml, t;
    mul128(m, N, &mh, &ml);
    t = hi + mh + (lo != 0 ? 1 : 0);
    if (t >= N) t -= N;
    return t;
}

static u128_t mmul(u128_t a, u128_t b, u128_t N, u128_t np)
{
    u128_t hi, lo;
    mul128(a, b, &hi, &lo);
    return redc(hi, lo, N, np);
}

static bool mr_prime(u128_t N)
{
    static const unsigned bases[13] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};
    u128_t d = N - 1, inv = N, np, r1, R2, one, mone;
    int s = 0, i, j, r;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (i = 0; i < 7; i++) inv *= (u128_t)2 - N * inv;
    np = (u128_t)0 - inv;
    r1 = ((u128_t)0 - N) % N;
    R2 = r1;
    for (i = 0; i < 128; i++) { R2 <<= 1; if (R2 >= N) R2 -= N; }
    one = r1;
    mone = N - r1;
    for (j = 0; j < 13; j++) {
        u128_t b = (u128_t)bases[j] % N, x, base, e;
        if (b == 0) continue;
        base = mmul(b, R2, N, np);
        x = one;
        e = d;
        while (e) {
            if (e & 1) x = mmul(x, base, N, np);
            base = mmul(base, base, N, np);
            e >>= 1;
        }
        if (x == one || x == mone) continue;
        for (r = 1; r < s; r++) {
            x = mmul(x, x, N, np);
            if (x == mone) break;
        }
        if (r >= s) return false;
    }
    return true;
}

#define TP_LIMIT 3000
static unsigned tprimes[TP_LIMIT];
static int ntp = -1;

static void init_tp(void)
{
    char comp[TP_LIMIT + 1] = {0};
    int i, j;
    ntp = 0;
    for (i = 2; i <= TP_LIMIT; i++) {
        if (comp[i]) continue;
        for (j = i * i; j <= TP_LIMIT; j += i) comp[j] = 1;
        if (i % 8 == 1) tprimes[ntp++] = (unsigned)i;
    }
}

static bool is_x4p1_prime(uint64_t x)
{
    u128_t N = (u128_t)x * x;
    int i;
    N = N * N + 1;
    for (i = 0; i < ntp; i++) {
        uint64_t p = tprimes[i], xm = x % p, q = (xm * xm) % p;
        if ((q * q + 1) % p == 0) return N == (u128_t)p;
    }
    return mr_prime(N);
}

static uint64_t *terms = NULL;
static size_t nterms = 0, cap = 0;
static uint64_t next_k = 1;

seq_t a(idx_t n)
{
    if (ntp < 0) init_tp();
    while (nterms < (size_t)n) {
        uint64_t k = next_k++;
        if (is_x4p1_prime(2 * k)) {
            if (nterms == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[nterms++] = k;
        }
    }
    return (seq_t)terms[n - 1];
}
