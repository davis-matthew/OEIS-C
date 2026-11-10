/*
 * A065374  Smallest k such that f(f(...f(k))) > 1, where f(k) = A065371(k) is
 *          iterated n times.
 * https://oeis.org/A065374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = smallest prime p with f^n(p) > 1, where f = A065371.
 *            Found by a search over a prime bitset. f is multiplicative and
 *            f(m) = 1 exactly when m is 3-smooth, so f^j(m) > 1 iff some
 *            prime factor q of m has f^j(q) > 1, which needs q >= a(j). For a
 *            prime q, f^j(q) = f^(j-1)(q - pi(q)). This lets t...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *A065374_W = NULL;   /* bit i set <=> 2i+1 prime */
static uint32_t *A065374_PRE = NULL; /* primes (odd) in words before w */
static uint64_t A065374_LIM = 0;     /* numbers < LIM covered, multiple of 128 */
static uint64_t A065374_A[80];
static int A065374_known = 0;

static int A065374_pc(uint64_t x) { return __builtin_popcountll(x); }

static void A065374_extend(uint64_t newlim)
{
    newlim = (newlim + 127) / 128 * 128;
    if (newlim <= A065374_LIM) return;
    size_t oldw = (size_t)(A065374_LIM / 128), nw = (size_t)(newlim / 128);
    uint64_t *w = (uint64_t *)realloc(A065374_W, nw * sizeof(uint64_t));
    if (!w) abort();
    A065374_W = w;
    uint32_t *pre = (uint32_t *)realloc(A065374_PRE, nw * sizeof(uint32_t));
    if (!pre) abort();
    A065374_PRE = pre;
    for (size_t i = oldw; i < nw; ++i) w[i] = ~(uint64_t)0;

    /* base primes up to sqrt(newlim) */
    uint64_t r = 1;
    while (r * r <= newlim) ++r;
    unsigned char *s = (unsigned char *)calloc((size_t)r + 1, 1);
    if (!s) abort();
    uint32_t *bp = (uint32_t *)malloc(((size_t)r + 1) * sizeof(uint32_t));
    if (!bp) abort();
    size_t bpc = 0;
    for (uint64_t i = 3; i <= r; i += 2) {
        if (s[i]) continue;
        bp[bpc++] = (uint32_t)i;
        for (uint64_t j = i * i; j <= r; j += 2 * i) s[j] = 1;
    }
    free(s);

    const size_t BLK = 4096;
    for (size_t wb = oldw; wb < nw; wb += BLK) {
        size_t we = wb + BLK < nw ? wb + BLK : nw;
        uint64_t lo = (uint64_t)wb * 128, hi = (uint64_t)we * 128;
        for (size_t k = 0; k < bpc; ++k) {
            uint64_t p = bp[k];
            if (p * p >= hi) break;
            uint64_t st = p * p;
            if (st < lo) {
                st = (lo + p - 1) / p * p;
                if (!(st & 1)) st += p;
            }
            for (uint64_t j = st; j < hi; j += 2 * p) {
                uint64_t i = j >> 1;
                w[i >> 6] &= ~((uint64_t)1 << (i & 63));
            }
        }
        if (wb == 0) w[0] &= ~(uint64_t)1; /* 1 is not prime */
    }
    free(bp);
    for (size_t i = oldw; i < nw; ++i)
        pre[i] = (i == 0) ? 0 : pre[i - 1] + (uint32_t)A065374_pc(w[i - 1]);
    A065374_LIM = newlim;
}

static bool A065374_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if (!(x & 1)) return false;
    uint64_t i = x >> 1;
    return (A065374_W[i >> 6] >> (i & 63)) & 1;
}

static uint64_t A065374_pi(uint64_t x)
{
    if (x < 2) return 0;
    if (x == 2) return 1;
    uint64_t i = (x - 1) >> 1;
    uint64_t wi = i >> 6, b = i & 63;
    uint64_t mask = (b == 63) ? ~(uint64_t)0 : (((uint64_t)1 << (b + 1)) - 1);
    return 1 + (uint64_t)A065374_PRE[wi] + (uint64_t)A065374_pc(A065374_W[wi] & mask);
}

/* true iff f^j(m) > 1 ; requires A[1..j] known */
static bool A065374_holds(uint64_t m, int j)
{
    if (j == 0) return m > 1;
    uint64_t t = A065374_A[j];
    if (m < t) return false;
    uint64_t cmax = m / t;
    for (uint64_t c = 1; c <= cmax; ++c) {
        if (m % c) continue;
        uint64_t q = m / c;
        if (A065374_isprime(q) && A065374_holds(q - A065374_pi(q), j - 1))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (A065374_LIM == 0) A065374_extend((uint64_t)1 << 20);
    A065374_A[0] = 2;
    while (A065374_known < (int)n) {
        int t = A065374_known + 1;
        uint64_t p = A065374_A[t - 1];
        if (p < 3) p = 3;
        if (!(p & 1)) ++p;
        for (;;) {
            if (p >= A065374_LIM) A065374_extend(A065374_LIM * 2);
            if (A065374_isprime(p) && A065374_holds(p - A065374_pi(p), t - 1))
                break;
            p += 2;
        }
        A065374_A[t] = p;
        A065374_known = t;
    }
    return (seq_t)A065374_A[(int)n];
}
