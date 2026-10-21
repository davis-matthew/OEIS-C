/*
 * A084548  Numbers k that have primitive roots less than k that are
 *          nonnegative perfect fifth powers.
 * https://oeis.org/A084548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 6393, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers k that have a primitive root r < k such that
   r = m^5 for some integer m (non‑negative fifth powers).
   The function a(n) returns the nth term (a(1)=2).  The implementation
   searches k in increasing order, storing the qualifying terms in a
   dynamic cache.  It uses a standard sieve for primes up to 2 000 000
   and performs modular exponentiation with 128‑bit arithmetic.  The
   code is valid C11 and also compiles as C++17. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd64.h"

/* ---------- prime sieve (up to 2 000 000) --------------------------- */
#define PRIME_LIMIT 2000000
static uint32_t primes[148933];  /* about pi(2M) */
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    bool *comp = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    for (int i = 0; i <= PRIME_LIMIT; ++i) comp[i] = false;
    for (int p = 2; p <= PRIME_LIMIT; ++p) {
        if (!comp[p]) {
            primes[prime_cnt++] = (uint32_t)p;
            if ((int64_t)p * p <= PRIME_LIMIT)
                for (int64_t q = (int64_t)p * p; q <= PRIME_LIMIT; q += p)
                    comp[q] = true;
        }
    }
    free(comp);
    primes_inited = true;
}

/* ---------- utility functions ------------------------------------- */

static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t cur = base % mod;
    while (exp) {
        if (exp & 1ULL) res = (unsigned __int128)res * cur % mod;
        cur = (unsigned __int128)cur * cur % mod;
        exp >>= 1;
    }
    return res;
}

/* ---------- prime factorisation ----------------------------------- */
static void factor_uint64(uint64_t n,
                          uint32_t *fpr, uint8_t *fexp,
                          int *fcnt)
{
    *fcnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            fpr[(*fcnt)] = p;
            fexp[(*fcnt)] = (uint8_t)e;
            ++(*fcnt);
        }
    }
    if (tmp > 1) {
        fpr[(*fcnt)] = (uint32_t)tmp;
        fexp[(*fcnt)] = 1;
        ++(*fcnt);
    }
}

/* ---------- test whether k satisfies the definition --------------- */
static bool qualifies(uint64_t k)
{
    if (k == 2)         /* 1^5 = 1 is a primitive root of 2 */
        return true;
    if (k == 4)         /* 4 has no primitive roots */
        return false;

    /* factor k */
    uint32_t fpr[20];
    uint8_t fexp[20];
    int fcnt;
    factor_uint64(k, fpr, fexp, &fcnt);

    /* check existence of primitive roots */
    bool has_root;
    if (k & 1) {                     /* odd k */
        has_root = (fcnt == 1);      /* odd prime power */
    } else {                         /* even k */
        if (k % 4 == 0) return false; /* 4 * p^e or 2^e, e>=2 -> no primitive roots */
        has_root = false;
        uint32_t odd_p = 0; int odd_e = 0;
        for (int i = 0; i < fcnt; ++i) {
            if (fpr[i] == 2) {
                if (fexp[i] != 1) return false;
            } else {
                odd_p = fpr[i]; odd_e = fexp[i];
                has_root = true;
            }
        }
    }
    if (!has_root) return false;

    /* compute φ(k) */
    uint64_t phi = 1;
    for (int i = 0; i < fcnt; ++i) {
        uint64_t p = fpr[i];
        int e = fexp[i];
        __int128 t = 1;
        for (int j = 1; j < e; ++j) t *= p;     /* p^(e‑1) */
        t *= (p - 1);                           /* p^(e‑1)*(p‑1) */
        phi *= (uint64_t)t;
    }
    if (phi % 5 == 0) return false;              /* φ(k) must not be divisible by 5 */

    /* factor φ(k) */
    uint32_t fphi[20];
    uint8_t fphi_exp[20];
    int fphi_cnt;
    factor_uint64(phi, fphi, fphi_exp, &fphi_cnt);

    /* test all fifth powers m^5 < k */
    for (uint64_t m = 1; ; ++m) {
        __int128 t = (__int128)m * m * m * m * m;   /* m^5 */
        if (t >= k) break;
        uint64_t r = (uint64_t)t;
        if (gcd64(r, k) != 1) continue;

        /* multiplicative order of r modulo k must equal φ(k) */
        uint64_t ord = phi;
        for (int i = 0; i < fphi_cnt; ++i) {
            uint64_t q = fphi[i];
            while (ord % q == 0 && modpow64(r, ord / q, k) == 1)
                ord /= q;
        }
        if (ord == phi) return true;
    }
    return false;
}

/* ---------- cache for sequence values ------------------------------ */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_test = 2;   /* next number to examine */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, not part of sequence */

    init_primes();                 /* ensure primes are ready */

    while (cache_len < (size_t)n) {
        if (qualifies(next_test)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap << 1 : 64;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_test;
        }
        ++next_test;
    }
    return cache[n - 1];
}
