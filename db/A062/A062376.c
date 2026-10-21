/*
 * A062376  Numbers k such that Euler phi(k) / Carmichael lambda(k) = 8.
 * https://oeis.org/A062376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th positive integer k such that φ(k)/λ(k) = 8,
 *            where φ is Euler's totient and λ is the Carmichael function
 *            (exponent of (ℤ/kℤ)×). (Search method) Numbers are tested
 *            sequentially; only those for which the ratio equals 8 are kept.
 *            The list is cached so that each element is computed at...
 * Range:     u8 n <= 7, u16 n <= 6515, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"

/* --- Utility ----------------------------------------------------- */

/* --- Prime sieve ------------------------------------------------- */
#define MAX_SIEVE 200000        /* enough for √(k) when k~10⁶ */
static uint32_t prime_buf[MAX_SIEVE];
static int prime_cnt = 0;
static int primes_built = 0;

static void build_primes(void) {
    if (primes_built) return;
    primes_built = 1;
    static unsigned char is_comp[MAX_SIEVE + 1];
    for (int i = 2; i <= MAX_SIEVE; ++i) {
        if (!is_comp[i]) {
            prime_buf[prime_cnt++] = (uint32_t)i;
            if ((uint64_t)i * i <= (uint64_t)MAX_SIEVE)
                for (uint64_t j = (uint64_t)i * i; j <= (uint64_t)MAX_SIEVE; j += i)
                    is_comp[(size_t)j] = 1;
        }
    }
}

/* --- Cache for sequence values ---------------------------------- */
#define CACHE_MAX 20000          /* enough for n up to ~10⁴ */
static seq_t cache[CACHE_MAX];
static int cache_len = 0;
static uint64_t last_checked = 0;     /* last integer examined */

/* --- Main function ---------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset is 1 */
    build_primes();

    /* generate terms until we have n of them */
    while (cache_len < (int)n) {
        ++last_checked;
        uint64_t m = last_checked;

        /* factorization */
        uint64_t tmp = m;
        struct { uint32_t p; unsigned exp; } fac[32];
        int fac_cnt = 0;
        for (int i = 0; i < prime_cnt && (uint64_t)prime_buf[i] * (uint64_t)prime_buf[i] <= tmp; ++i) {
            uint32_t p = prime_buf[i];
            if (tmp % p == 0) {
                unsigned e = 0;
                do { tmp /= p; ++e; } while (tmp % p == 0);
                fac[fac_cnt].p = p; fac[fac_cnt].exp = e; ++fac_cnt;
            }
        }
        if (tmp > 1) {
            fac[fac_cnt].p = (uint32_t)tmp; fac[fac_cnt].exp = 1; ++fac_cnt;
        }

        /* φ(m) */
        __uint128_t phi = m;
        for (int i = 0; i < fac_cnt; ++i) {
            uint32_t p = fac[i].p;
            phi = phi / p * (p - 1U);
        }

        /* λ(m) : lcm of λ(p^e) */
        __uint128_t lam = 1;
        for (int i = 0; i < fac_cnt; ++i) {
            uint32_t p = fac[i].p;
            unsigned e = fac[i].exp;
            __uint128_t part;
            if (p == 2) {
                if (e == 1)      part = 1;
                else if (e == 2) part = 2;
                else             part = ((unsigned long long)1) << (e - 2); /* 2^(e-2) */
            } else {
                /* λ(p^e) = p^(e-1)*(p-1) */
                __uint128_t pow = 1;
                for (unsigned k = 1; k <= e - 1; ++k) pow *= p;
                part = pow * (p - 1U);
            }
            /* lcm: lam = lam / gcd(lam,part) * part */
            uint64_t g = gcd_u64((uint64_t)lam, (uint64_t)part);
            lam = lam / g * part;
        }

        /* check φ/λ == 8 */
        if (lam != 0) {
            uint64_t ratio = (uint64_t)(phi / lam);
            if (ratio == 8) {
                if (cache_len < CACHE_MAX)
                    cache[cache_len++] = (seq_t)m;
            }
        }
    }

    return cache[n - 1];
}
