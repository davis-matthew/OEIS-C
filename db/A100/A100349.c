/*
 * A100349  Numbers n such that n-2^k is a prime or semiprime for all k > 0
 *          with 2^k < n.
 * https://oeis.org/A100349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 449, u32 n <= 1265, u64 n <= 1265,
 *            u128 n <= 1265, big any n
 * Validated: standard, pass, 76 terms through n = 1247 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is a term iff n - 2^k is a prime or a semiprime (Omega 1 or 2) for every
 * k >= 1 with 2^k < n. The candidates are taken in windows of 2^20; for every
 * number from 2^16 below the window to its end a sieve counts the prime
 * factors p <= sqrt(end) with multiplicity (one count per prime power p^e
 * dividing it) and adds up their logs, so the cofactor left is 1 or a single
 * prime > sqrt(end) (log >= 10), told apart by the log of the number minus
 * the sum. That settles 2^k <= 2^16 by lookup; the few candidates that pass
 * are tested for the larger k directly (exact 64-bit prime test; x is a
 * semiprime iff it is composite and either x/d is prime for its least prime
 * factor d <= cbrt(x), or it has no factor <= cbrt(x)). The terms go past
 * 2e9, so the original, which tested every candidate by trial division in
 * seq_t, timed out. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

#define W (1u << 20)                 /* candidates per window */
#define BACK (1u << 16)              /* the sieved range starts this far below */

static bool prime_or_semiprime(uint64_t x)
{
    if (x < 2) return false;
    if (is_prime_u64(x)) return true;
    for (uint64_t d = 2; d * d * d <= x; ++d)
        if (x % d == 0) return is_prime_u64(x / d);
    return true;                     /* composite, no factor <= cbrt(x) */
}

static uint32_t *P = NULL;           /* primes up to plim */
static float *LP = NULL;             /* their logs */
static size_t np = 0;
static uint64_t plim = 0;

static void ensure_primes(uint64_t lim)
{
    if (lim <= plim) return;
    lim = lim < 2 * plim ? 2 * plim : lim;
    unsigned char *c = (unsigned char *)calloc(lim + 1, 1);
    uint32_t *t = (uint32_t *)malloc((lim / 2 + 2) * sizeof *t);
    float *l = (float *)malloc((lim / 2 + 2) * sizeof *l);
    if (!c || !t || !l) abort();
    np = 0;
    for (uint64_t i = 2; i <= lim; ++i) {
        if (c[i]) continue;
        l[np] = (float)log((double)i);
        t[np++] = (uint32_t)i;
        for (uint64_t j = i * i; j <= lim; j += i) c[j] = 1;
    }
    free(c);
    free(P);
    free(LP);
    P = t;
    LP = l;
    plim = lim;
}

/* ok[i] = Omega(lo + i) is 1 or 2, for lo <= lo + i < hi */
static void sieve(uint64_t lo, uint64_t hi, unsigned char *ok, float *sum, unsigned char *cnt)
{
    uint64_t r = 1;
    while ((r + 1) * (r + 1) < hi) ++r;
    ensure_primes(r + 1);
    size_t len = (size_t)(hi - lo);
    for (size_t i = 0; i < len; ++i) { sum[i] = 0; cnt[i] = 0; }
    for (size_t j = 0; j < np && (uint64_t)P[j] * P[j] < hi; ++j) {
        uint64_t p = P[j];
        float lp = LP[j];
        for (uint64_t q = p; q < hi; q *= p) {           /* each power p^e */
            for (uint64_t m = (lo + q - 1) / q * q; m < hi; m += q) {
                size_t i = (size_t)(m - lo);
                sum[i] += lp;
                if (cnt[i] < 3) ++cnt[i];
            }
            if (q > hi / p) break;
        }
    }
    for (size_t i = 0; i < len; ++i) {
        uint64_t m = lo + i;
        unsigned c = cnt[i];
        if (m >= 2 && (float)log((double)m) - sum[i] > 5.0f) ++c;   /* a prime cofactor */
        ok[i] = m >= 2 && (c == 1 || c == 2);
    }
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t next = 4;        /* the first candidate not tested */
    static unsigned char *ok = NULL, *cn = NULL;
    static float *sum = NULL;
    if (n == 0) return (seq_t)0;
    if (!ok) {
        ok = (unsigned char *)malloc(W + BACK);
        cn = (unsigned char *)malloc(W + BACK);
        sum = (float *)malloc((W + BACK) * sizeof *sum);
        if (!ok || !cn || !sum) abort();
    }
    while (cnt < (size_t)n) {
        uint64_t L = next, lo = L > BACK ? L - BACK : 0, hi = L + W;
        sieve(lo, hi, ok, sum, cn);
        for (uint64_t c = L; c < hi && cnt < (size_t)n; ++c) {
            bool good = true;
            uint64_t p = 2;
            for (; p < c && p <= BACK; p <<= 1)
                if (!ok[c - p - lo]) { good = false; break; }
            for (; good && p < c; p <<= 1)
                if (!prime_or_semiprime(c - p)) good = false;
            next = c + 1;
            if (!good) continue;
            if (cnt == cap) {
                cap = cap ? 2 * cap : 128;
                seq_t *t = mem_resize(seq_t, terms, cap);
                if (!t) abort();
                terms = t;
            }
            terms[cnt++] = (seq_t)c;
        }
    }
    return terms[n - 1];
}
