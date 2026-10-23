/*
 * A185103  Smallest k > 1 such that k^(n-1) == 1 (mod n^2).
 * https://oeis.org/A185103
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A185103:
   a(n) = smallest k > 1 such that k^(n-1) ≡ 1 (mod n^2). */

/* The implementation uses prime–power factorisation of n^2,
   primitive roots for odd prime powers, and the special treatment
   for 2‑power modulus.  It combines the local solutions by
   the Chinese Remainder Theorem, keeping only the minimal value
   found.  The routine is fully self‑contained and works for any
   seq_t/idx_t defined by the build system. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_uint64.h"

/* ------------------------------------------------------------------ */
/* Basic helper functions (all operate on 64‑bit integers).             */
/* ------------------------------------------------------------------ */

static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

/* Extended Euclidean algorithm – modular inverse (b mod mod).          */
static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t t0 = 0, t1 = 1;
    int64_t r0 = mod, r1 = a % mod;
    while (r1 != 0) {
        int64_t q = r0 / r1;
        int64_t r2 = r0 - q * r1;  r0 = r1;  r1 = r2;
        int64_t t2 = t0 - q * t1;  t0 = t1;  t1 = t2;
    }
    if (r0 != 1) return 0;          /* inverse does not exist */
    if (t0 < 0) t0 += mod;
    return (uint64_t)t0;
}

/* ------------------------------------------------------------------ */
/* Small prime sieve (up to 10000 – enough for factorising up to 10^8)*/
/* ------------------------------------------------------------------ */

#define PRIMES_MAX 10000
static uint64_t primes[PRIMES_MAX];
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_cnt) return;
    int sieve[PRIMES_MAX + 1] = {0};
    for (int i = 2; i <= PRIMES_MAX; ++i) {
        if (!sieve[i]) {
            primes[primes_cnt++] = i;
            if ((long long)i * i <= PRIMES_MAX)
                for (int j = i * i; j <= PRIMES_MAX; j += i)
                    sieve[j] = 1;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Factorisation helpers                                             */
/* ------------------------------------------------------------------ */

/* Factor n into distinct prime factors; primes_out holds primes,
   cnt_out the number of distinct primes.  Exponents are ignored. */
static void factor_distinct(uint64_t n, uint64_t *primes_out, size_t *cnt_out)
{
    size_t cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            primes_out[cnt++] = primes[i];
            while (tmp % primes[i] == 0) tmp /= primes[i];
        }
    }
    if (tmp > 1) primes_out[cnt++] = tmp;
    *cnt_out = cnt;
}

/* Factor n into primes (with exponents).  primes_out holds primes,
   exps_out holds corresponding exponents, cnt_out the number of
   distinct primes. */
static void factor_with_exps(uint64_t n, uint64_t *primes_out,
                             int *exps_out, size_t *cnt_out)
{
    size_t cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            int e = 0;
            while (tmp % primes[i] == 0) { tmp /= primes[i]; ++e; }
            primes_out[cnt] = primes[i];
            exps_out[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        primes_out[cnt] = tmp;
        exps_out[cnt]   = 1;
        ++cnt;
    }
    *cnt_out = cnt;
}

/* ------------------------------------------------------------------ */
/* Primitive root finder for odd prime powers                        */
/* ------------------------------------------------------------------ */

static uint64_t find_primitive_root(uint64_t p, uint64_t e)
{
    /* modulus m = p^e (odd prime power) */
    uint64_t m = 1;
    for (uint64_t i = 0; i < e; ++i) m *= p;

    /* Euler totient φ(m) = p^e - p^(e-1) = p^(e-1)*(p-1) */
    uint64_t phi = m - m / p;

    /* Get distinct prime factors of φ */
    uint64_t facs[64];
    size_t fcnt;
    factor_distinct(phi, facs, &fcnt);

    /* Search smallest g such that g^(phi/q) ≠ 1 for all q|phi */
    for (uint64_t g = 2; g < m; ++g) {
        int ok = 1;
        for (size_t i = 0; i < fcnt; ++i) {
            uint64_t q = facs[i];
            if (powmod_uint64(g, phi / q, m) == 1) { ok = 0; break; }
        }
        if (ok) return g;
    }
    return 1; /* Should never happen for odd prime powers */
}

/* ------------------------------------------------------------------ */
/* Chinese Remainder Theorem (pair of coprime moduli)                  */
/* ------------------------------------------------------------------ */

static void crt_pair(uint64_t a1, uint64_t m1,
                     uint64_t a2, uint64_t m2,
                     uint64_t *a_out, uint64_t *m_out)
{
    uint64_t inv = modinv_uint64(m1 % m2, m2);
    uint64_t diff;
    uint64_t r1 = a1 % m2;
    if (a2 >= r1) diff = a2 - r1;
    else          diff = a2 + m2 - r1;
    uint64_t t = (diff * inv) % m2;
    *a_out = a1 + m1 * t;
    *m_out = m1 * m2;
}

/* ------------------------------------------------------------------ */
/* Recursive search for minimal solution with pruning                */
/* ------------------------------------------------------------------ */

static void search_min(int idx, uint64_t curr_mod, uint64_t curr_res,
                       uint64_t n_minus1,
                       uint64_t *best,
                       uint64_t *mods, uint64_t *p_list,
                       int *e_list, int num_factors)
{
    if (idx == num_factors) {
        if (curr_res > 1 && (*best == 0 || curr_res < *best))
            *best = curr_res;
        return;
    }

    uint64_t p = p_list[idx];
    int e   = e_list[idx];
    uint64_t m = 1;
    for (int i = 0; i < 2 * e; ++i) m *= p;      /* modulus = p^(2e) */

    /* Compute all local solutions modulo m */
    if (p == 2) {
        /* Special handling for 2^k (k >= 2).  Here m = 2^(2e). */
        uint64_t k_power = 2 * e; /* exponent of modulus */
        uint64_t ord = (k_power <= 2) ? 1 : 1ULL << (k_power - 2); /* order of group generator 5 */
        uint64_t g2   = gcd_uint64(ord, n_minus1);
        int b_options = (n_minus1 & 1) ? 1 : 2; /* (-1)^b allowed only if n-1 even */

        uint64_t step = ord / g2;
        for (uint64_t a = 0; a < g2; ++a) {
            uint64_t val = powmod_uint64(5, a * step, m);
            for (int b = 0; b < b_options; ++b) {
                uint64_t r = (b == 0) ? val : (m - val) % m; /* (-1)^b * val */
                uint64_t new_res, new_mod;
                crt_pair(curr_res, curr_mod, r, m, &new_res, &new_mod);
                if (*best && new_res >= *best) continue; /* pruning */
                search_min(idx + 1, new_mod, new_res, n_minus1,
                           best, mods, p_list, e_list, num_factors);
            }
        }
    } else {
        /* Odd prime power */
        uint64_t primitive = find_primitive_root(p, 2 * e);
        uint64_t phi = m - m / p;
        uint64_t g   = gcd_uint64(phi, n_minus1);
        uint64_t step = phi / g;
        for (uint64_t t = 0; t < g; ++t) {
            uint64_t r = powmod_uint64(primitive, t * step, m);
            uint64_t new_res, new_mod;
            crt_pair(curr_res, curr_mod, r, m, &new_res, &new_mod);
            if (*best && new_res >= *best) continue;
            search_min(idx + 1, new_mod, new_res, n_minus1,
                       best, mods, p_list, e_list, num_factors);
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    init_primes();

    if (n < 2) return (seq_t)0;               /* undefined for n < 2 */

    /* Factorise n: we need prime powers of n to build n^2 */
    uint64_t primes_fac[32];
    int exps_fac[32];
    size_t num_fac;
    factor_with_exps((uint64_t)n, primes_fac, exps_fac, &num_fac);

    /* Build arrays for search */
    uint64_t *mods = (uint64_t*)malloc(num_fac * sizeof(uint64_t));
    for (size_t i = 0; i < num_fac; ++i) {
        uint64_t p = primes_fac[i];
        int e = exps_fac[i];
        uint64_t m = 1;
        for (int j = 0; j < 2 * e; ++j) m *= p;
        mods[i] = m;
    }

    /* Perform recursive search for minimal k */
    uint64_t best = 0; /* 0 means not found yet */
    search_min(0, 1, 0, (uint64_t)(n - 1),
               &best, mods, primes_fac, exps_fac, (int)num_fac);

    /* If no solution >1 found inside bounds, the theoretical upper
       bound gives k = n^2 + 1.  This case should not occur, but we
       handle it for completeness. */
    if (!best) {
        uint64_t prod = 1;
        for (size_t i = 0; i < num_fac; ++i)
            prod *= mods[i];
        best = prod + 1;   /* n^2 + 1 */
    }

    free(mods);
    return (seq_t)best;
}
