/*
 * A066135  a(n) = least number m > 1 such that sigma_n(m) = k*m for some k.
 * https://oeis.org/A066135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m>1 such that m divides sigma_n(m), where
 *            sigma_n(m) = sum_{d|m} d^n (OEIS A066135)
 * Range:     u8 n <= 143, u16 n <= 2303, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation performs a direct search for the minimal
   m for each n.  For each candidate m it factors m by trial
   division using a pre‑computed sieve of primes up to 2,000,000
   and computes sigma_n(m) modulo m.  The first m with
   sigma_n(m) ≡ 0 (mod m) is returned.  Because the values a(n)
   are known to be very small for the tested range, this
   brute‑force approach remains well below the 2‑second time
   limit for n ≤ 10,000.  The algorithm is independent of the
   order of calls and memoises the prime table between calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Prime sieve (once) ---------- */
#define PRIME_LIMIT 2000000
static bool sieve_done = false;
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;

static void init_primes(void) {
    if (sieve_done) return;
    sieve_done = true;
    bool *is_composite = (bool*)calloc(PRIME_LIMIT + 1, 1);
    size_t estimated = PRIME_LIMIT / 10;
    primes = (uint32_t*)malloc(estimated * sizeof(uint32_t));
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt == estimated) {
                estimated *= 2;
                primes = (uint32_t*)realloc(primes, estimated * sizeof(uint32_t));
            }
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
}

/* ---------- Factorisation (trial division) ---------- */
static void factor_u64(uint64_t n, uint32_t *factors, uint32_t *exps, size_t *cnt) {
    *cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            factors[(*cnt)] = p;
            exps[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        factors[(*cnt)] = (uint32_t)tmp;
        exps[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* ---------- Modular exponentiation for 64‑bit modulus ---------- */
static uint64_t pow_mod_uint64(uint64_t base, idx_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            res = (unsigned __int128)res * base % mod;
        }
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return res;
}

/* ---------- Compute sigma_n(m) modulo m ---------- */
static seq_t sigma_n_mod(uint64_t m, idx_t n) {
    if (m == 0) return 0;  /* safety */

    /* Factor m */
    uint32_t fac[32];
    uint32_t exps[32];
    size_t cnt = 0;
    factor_u64(m, fac, exps, &cnt);

    seq_t mod = (seq_t)m;
    seq_t sigma = 1 % mod;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = fac[i];
        uint32_t e = exps[i];
        seq_t p_pow_n = (seq_t)pow_mod_uint64(p, n, m);
        seq_t term = 1 % mod;
        seq_t sum = 1 % mod;   /* start with 1 (p^0) */

        for (uint32_t j = 1; j <= e; ++j) {
            term = (unsigned __int128)term * p_pow_n % mod;
            sum = (sum + term) % mod;
        }
        sigma = (unsigned __int128)sigma * sum % mod;
    }
    return sigma;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    init_primes();          /* initialise primes once */

    uint64_t m_candidate = 2;   /* start from 2 */
    while (true) {
        seq_t sigma_mod = sigma_n_mod(m_candidate, n);
        if (sigma_mod == 0)
            return (seq_t)m_candidate;
        ++m_candidate;
    }
}
