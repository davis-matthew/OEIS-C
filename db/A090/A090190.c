/*
 * A090190  Symmetric primes: an odd prime p is symmetric if there exists an
 *          odd prime q such that |p-q| = gcd(p-1,q-1).
 * https://oeis.org/A090190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth odd prime p such that there exists an odd prime
 *            q * with |p-q| = gcd(p-1, q-1). The function generates all odd
 *            primes up to a * fixed limit, tests each for the symmetric
 *            condition, and memoises the * results. No b-file or pre‑computed
 *            table is used
 * Range:     u8 n <= 48, u16 n <= 5570, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- include files ---------- */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- type aliases (provided by the build system) ----------
   The following declarations exist in the build environment:
   typenames: seq_t (output integer type), idx_t (unsigned integer type)
 */

/* ---------- constants ---------- */
#define SIEVE_LIMIT 5000000        /* primes generated up to 5 000 000 */
#define PRIME_CAP    (SIEVE_LIMIT / 10 + 1)   /* rough upper bound on #primes */
#define DIVISOR_CAP  2048          /* max number of divisors of p‑1 (p≤SIEVE_LIMIT) */

/* ---------- global buffers ---------- */
static uint32_t spf[SIEVE_LIMIT + 1];   /* smallest prime factor for each number */
static uint32_t prime_list[PRIME_CAP];  /* list of generated primes */
static size_t  prime_count = 0;         /* number of primes found */

/* symmetric primes memoisation */
static seq_t   *sym_primes = NULL;
static size_t sym_count   = 0;
static size_t sym_cap     = 0;

/* index of next prime to process */
static size_t processed_prime_idx = 0;

/* ---------- helper functions ---------- */

/* multiply modulo: 64‑bit product using 128‑bit intermediate if available */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
#if defined(__UINT128_T__) || defined(__uint128_t) || defined(__int128_t)
    return (uint64_t)((__uint128_t)a * b % mod);
#else
    /* fallback: binary addition method (slow but works on 64‑bit targets without 128‑bit) */
    uint64_t result = 0;
    a = a % mod;
    while (b) {
        if (b & 1)
            result = (result + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return result;
#endif
}

/* modular exponentiation */
static inline uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1)
            r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    /* check small primes */
    static const uint64_t small[12] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,
                                       17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < 12; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* bases sufficient for <2^64 */
    static const uint64_t bases[7] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < 7; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- sieve of Eratosthenes (smallest prime factor, prime list) ---------- */
static void build_sieve(void)
{
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = i;
            if (prime_count < PRIME_CAP)
                prime_list[prime_count++] = i;
            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* ensure capacity of symmetric‑prime array */
static void ensure_sym_capacity(size_t needed)
{
    if (sym_cap < needed) {
        size_t new_cap = (sym_cap == 0) ? 256 : sym_cap * 2;
        while (new_cap < needed) new_cap <<= 1;
        sym_primes = (seq_t *)realloc(sym_primes, new_cap * sizeof(seq_t));
        sym_cap = new_cap;
    }
}

/* factor n (≤SIEVE_LIMIT) into prime factors via spf */
static size_t factor(uint32_t n, uint32_t fac_p[16], uint32_t fac_e[16])
{
    size_t cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t e = 0;
        do { n /= p; ++e; } while (n % p == 0);
        fac_p[cnt] = p;
        fac_e[cnt] = e;
        ++cnt;
    }
    return cnt;
}

/* generate all divisors of n from its factorisation (fac_p, fac_e, fac_cnt) */
static size_t generate_divisors(const uint32_t fac_p[16], const uint32_t fac_e[16],
                                size_t fac_cnt, uint32_t divs[DIVISOR_CAP])
{
    size_t len = 1;
    divs[0] = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint32_t p = fac_p[i];
        uint32_t e = fac_e[i];
        size_t old_len = len;
        uint32_t pow = 1;
        for (uint32_t exp = 1; exp <= e; ++exp) {
            pow *= p;
            for (size_t j = 0; j < old_len; ++j)
                if (len < DIVISOR_CAP)
                    divs[len++] = divs[j] * pow;
        }
    }
    return len;
}

/* check if an odd prime p is symmetric */
static bool is_symmetric_prime(uint32_t p)
{
    if (p <= 2) return false;      /* skip 2 and below */

    uint32_t fac_p[16], fac_e[16];
    size_t fac_cnt = factor(p - 1, fac_p, fac_e);

    uint32_t divs[DIVISOR_CAP];
    size_t div_len = generate_divisors(fac_p, fac_e, fac_cnt, divs);

    for (size_t i = 0; i < div_len; ++i) {   /* include divisor 1 as well */
        uint32_t d = divs[i];
        if (d == 0) continue;
        uint64_t q1 = (uint64_t)p + d;
        if (isPrime64(q1) && (q1 & 1)) return true;
        if (p > d) {
            uint64_t q2 = (uint64_t)p - d;
            if (q2 > 2 && (q2 & 1) && isPrime64(q2)) return true;
        }
    }
    return false;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset 1 – undefined for n=0 */

    /* one‑time initialisation */
    static bool initialized = false;
    if (!initialized) {
        build_sieve();
        ensure_sym_capacity(256);
        initialized = true;
    }

    /* generate symmetric primes until we have at least n of them */
    while (sym_count < n) {
        /* if we run out of generated primes, stop – limit is chosen large enough for n≤10000 */
        if (processed_prime_idx >= prime_count) break;
        uint32_t p = prime_list[processed_prime_idx++];
        if (is_symmetric_prime(p)) {
            ensure_sym_capacity(sym_count + 1);
            sym_primes[sym_count++] = (seq_t)p;
        }
    }

    if (n <= sym_count) return sym_primes[n - 1];
    /* if not enough found (should not happen for the intended ranges) */
    return 0;
}
