/*
 * A066257  a(n) is the smallest number >1 of the form (k^2+n)/(n^2+k).
 * https://oeis.org/A066257
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest integer r>1 such that (k^2+n)/(n^2+k)=r for some
 *            integer k
 * Range:     u8 n <= 24, u16 n <= 369, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code enumerates all divisors d of N=n^4+n and evaluates r = d-2*n^2+N/d. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd64.h"

/* ---------- 64‑bit arithmetics utilities ---------- */

static inline uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* ---------- Miller‑Rabin primality test (deterministic for 64‑bit) ---------- */

static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                              450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = 1, pow = d;
        uint64_t base = a;
        while (pow) {
            if (pow & 1) x = mulmod64(x, base, n);
            base = mulmod64(base, base, n);
            pow >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- Pollard‑Rho factorisation ---------- */

static uint64_t pollard_rho(uint64_t n)
{
    if (n % 2ULL == 0ULL) return 2ULL;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod64(x, x, n) + c) % n;
            y = (mulmod64(y, y, n) + c) % n;
            y = (mulmod64(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd64(diff, n);
        }
        if (d != n) return d;
        ++c;   /* try a different constant if failure */
    }
}

/* ---------- Recursive factorisation into prime factors ---------- */

static void factor_rec(uint64_t n, uint64_t *fac, size_t *cnt)
{
    if (n == 1ULL) return;
    if (is_prime64(n)) { fac[(*cnt)++] = n; return; }
    uint64_t d = pollard_rho(n);
    factor_rec(d, fac, cnt);
    factor_rec(n / d, fac, cnt);
}

/* ---------- Generate all divisors from prime powers ---------- */

static void gen_divs_rec(const uint64_t *primes, const uint32_t *exps,
                         size_t idx, uint64_t cur,
                         uint64_t **divs, size_t *cnt, size_t *cap)
{
    if (idx == 0) {
        if (*cnt == *cap) {
            *cap = (*cap == 0) ? 4 : (*cap << 1);
            *divs = (uint64_t *)realloc(*divs, (*cap) * sizeof(uint64_t));
        }
        (*divs)[(*cnt)++] = cur;
        return;
    }
    uint64_t p = primes[idx - 1];
    uint64_t value = cur;
    for (uint32_t e = 0; e <= exps[idx - 1]; ++e) {
        gen_divs_rec(primes, exps, idx - 1, value, divs, cnt, cap);
        value *= p;
    }
}

static void generate_divisors(const uint64_t *primes, const uint32_t *exps,
                              size_t num, uint64_t **divs, size_t *cnt)
{
    size_t cap = 0;
    *divs = NULL;
    *cnt = 0;
    gen_divs_rec(primes, exps, num, 1ULL, divs, cnt, &cap);
}

/* ---------- Main OEIS function ---------- */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined for n < 2 */

    /* Convert idx_t to 64‑bit integer */
    uint64_t n64 = (uint64_t)n;

    /* Compute N = n^4 + n */
    uint64_t n2 = n64 * n64;                    /* n^2 */
    uint64_t n4 = (uint64_t)(((__uint128_t)n2 * n2));   /* n^4 */
    uint64_t N  = n4 + n64;                      /* n^4 + n */

    /* Factor N */
    uint64_t factors[64];
    size_t fact_cnt = 0;
    factor_rec(N, factors, &fact_cnt);

    /* Count powers of each distinct prime */
    uint64_t uniq_primes[64];
    uint32_t uniq_exps[64];
    size_t uniq_cnt = 0;
    for (size_t i = 0; i < fact_cnt; ++i) {
        bool found = false;
        for (size_t j = 0; j < uniq_cnt; ++j) {
            if (factors[i] == uniq_primes[j]) {
                uniq_exps[j]++;
                found = true;
                break;
            }
        }
        if (!found) {
            uniq_primes[uniq_cnt] = factors[i];
            uniq_exps[uniq_cnt] = 1;
            ++uniq_cnt;
        }
    }

    /* Generate all divisors of N */
    uint64_t *divs = NULL;
    size_t div_cnt = 0;
    generate_divisors(uniq_primes, uniq_exps, uniq_cnt, &divs, &div_cnt);

    /* Find minimal r > 1 */
    uint64_t best = UINT64_MAX;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        /* r = d - 2*n^2 + N/d */
        uint64_t nd = n2 * 2ULL;
        uint64_t q  = N / d;
        uint64_t r  = d - nd + q;
        if (r > 1 && r < best) best = r;
    }

    free(divs);
    return (seq_t)best;
}
