/*
 * A105901  Primes for which -29 is a primitive root.
 * https://oeis.org/A105901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime p such that -29 is a primitive root modulo p;
   computed by testing primes sequentially for primitivity */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper: small primes list (used for primality test & factorisation) */
static int primes_array[120000];
static int primes_count = 0;
static unsigned char *primes_is_composite = NULL;

static void init_primes(void) {
    if (primes_is_composite) return;                 /* already initialised */
    const int LIMIT = 100000;                        /* enough for sqrt(10^7) */
    primes_is_composite = (unsigned char*)calloc(LIMIT + 1, 1);
    for (int i = 2; i <= LIMIT; ++i) {
        if (!primes_is_composite[i]) {
            if (primes_count < 120000) {
                primes_array[primes_count++] = i;
            }
            if ((int64_t)i * i <= LIMIT) {
                for (int j = i * i; j <= LIMIT; j += i)
                    primes_is_composite[j] = 1;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Basic primality test (works up to 2^64, but fast for our range) */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    for (int i = 0; i < primes_count; ++i) {
        uint32_t p = primes_array[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Construct distinct prime factors of m (m > 1) */
static void factor_distinct_prime_factors(uint64_t m, uint64_t *factors, int *cnt) {
    *cnt = 0;
    for (int i = 0; i < primes_count; ++i) {
        uint64_t p = primes_array[i];
        if (p * p > m) break;
        if (m % p == 0) {
            if (*cnt < 64) factors[(*cnt)++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1 && *cnt < 64) factors[(*cnt)++] = m;
}

/* ------------------------------------------------------------------ */
/* Modular exponentiation using 128‑bit intermediate */
static uint64_t modpow_uint64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t res = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1) res = (unsigned __int128)res * a % mod;
        a = (unsigned __int128)a * a % mod;
        e >>= 1;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Store good primes found so far */
static uint64_t *good_primes = NULL;
static size_t good_count   = 0;
static size_t good_capacity = 0;

/* Start searching from this value in successive calls */
static uint64_t next_candidate = 2;

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;                 /* undefined for n<1 */

    /* Initialise helpers */
    if (!primes_is_composite) init_primes();
    if (!good_primes) {
        good_capacity = 1024;
        good_primes = (uint64_t*)malloc(good_capacity * sizeof(uint64_t));
        good_count = 0;
    }

    /* Find until we have at least n good primes */
    while (good_count < (size_t)n) {
        /* Find next prime candidate */
        while (true) {
            uint64_t p = next_candidate;
            ++next_candidate;                   /* increment for future */

            if (p < 2) continue;
            if (!is_prime_uint64(p)) continue;
            if (p == 29) continue;              /* -29 ≡ 0 mod 29 */

            /* base = (-29) mod p */
            uint64_t base = p - (29 % p);
            if (base == 0) continue;            /* safety, only p==29 */

            /* Factor p-1 */
            uint64_t factors[64];
            int fac_cnt;
            factor_distinct_prime_factors(p - 1, factors, &fac_cnt);

            /* Primitive root test */
            bool ok = true;
            for (int i = 0; i < fac_cnt; ++i) {
                uint64_t exp = (p - 1) / factors[i];
                if (modpow_uint64(base, exp, p) == 1) {
                    ok = false; break;
                }
            }

            if (ok) {
                /* Store the good prime */
                if (good_count == good_capacity) {
                    good_capacity *= 2;
                    good_primes = (uint64_t*)realloc(good_primes,
                                                     good_capacity * sizeof(uint64_t));
                }
                good_primes[good_count++] = p;
                break;                           /* found one, exit inner loop */
            }
        }
    }

    return (seq_t)good_primes[n - 1];
}
