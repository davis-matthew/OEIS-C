/*
 * A070683  Smallest m in range 1..phi(2n+1) such that 12^m == 1 mod 2n+1, or
 *          0 if no such number exists.
 * https://oeis.org/A070683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest m∈[1,φ(2n+1)] such that 12^m ≡ 1 (mod 2n+1), or
 *            0 if no such m exists (i.e., gcd(12,2n+1) ≠ 1)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_phi_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------- standard helpers --------------------------------------- */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/compute_phi_uint64.h"

/* modular exponentiation (assumes mod > 1, exp fits in 64 bits) */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* ------------- prime sieve -------------------------------------------- */
#define SIEVE_LIMIT 50000
static uint32_t primes[SIEVE_LIMIT];
static size_t primes_cnt = 0;
static bool sieve_done = false;

static void build_sieve(void)
{
    if (sieve_done) return;
    bool is_composite[SIEVE_LIMIT];
    for (size_t i = 0; i < SIEVE_LIMIT; ++i) is_composite[i] = false;
    for (size_t i = 2; i < SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if (i * i < SIEVE_LIMIT) {
                for (size_t j = i * i; j < SIEVE_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    sieve_done = true;
}

/* ------------- integer factorisation ---------------------------------- */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    uint64_t d = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= d; ++i) {
        if (d % primes[i] == 0) {
            uint64_t p = primes[i];
            uint64_t e = 0;
            while (d % p == 0) {
                d /= p;
                ++e;
            }
            pr[c] = p;
            exp[c] = e;
            ++c;
        }
    }
    if (d > 1) {
        pr[c] = d;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* ------------- multiplicative order ----------------------------------- */
static uint64_t minimal_order(uint64_t base, uint64_t mod)
{
    /* factor mod to get φ(mod) */
    uint64_t pfs[32], pexp[32];
    size_t pcnt;
    factor_uint64(mod, pfs, pexp, &pcnt);
    uint64_t phi = compute_phi_uint64(mod, pfs, pexp, pcnt);

    /* factor φ(mod) */
    uint64_t gfs[32], gexp[32];
    size_t gcnt;
    factor_uint64(phi, gfs, gexp, &gcnt);

    /* start with the full order = φ(mod) and reduce it */
    uint64_t order = phi;
    for (size_t i = 0; i < gcnt; ++i) {
        uint64_t q = gfs[i];
        while (order % q == 0) {
            uint64_t test = order / q;
            if (mod_pow_uint64(base, test, mod) == 1)
                order = test;
            else
                break;
        }
    }
    return order;
}

/* ------------- sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    build_sieve();

    uint64_t mod = 2ULL * (uint64_t)n + 1ULL;          /* 2n+1 */
    if (mod <= 1ULL) return (seq_t)0;                  /* n = 0 gives mod = 1 */

    /* gcd(12,mod) = 1 iff mod % 3 != 0, because mod is odd */
    if (mod % 3ULL == 0ULL) return (seq_t)0;

    uint64_t ord = minimal_order(12ULL, mod);
    /* minimal_order will return 0 only if something wrong, guard anyway */
    return (seq_t)ord;
}
