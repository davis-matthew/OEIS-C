/*
 * A129832  Integers n such that the n-th cyclotomic polynomial Phi(n) is
 *          irreducible over GF(2).
 * https://oeis.org/A129832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A129832 – integers n such that Φ(n) is irreducible over GF(2).
    The sequence consists of 1, 2 and numbers that are odd prime powers
    p^e or twice such powers where p is an odd prime for which 2 is a
    primitive root modulo p.
    This implementation generates the sequence in increasing order by
    testing each integer in turn. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---------- helpers for modular arithmetic ---------- */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1u)
            result = (uint64_t)((__uint128_t)result * b % mod);
        b = (uint64_t)((__uint128_t)b * b % mod);
        exp >>= 1u;
    }
    return result;
}

/* ---------- prime sieve and caching ---------- */
#define MAX_PRIMES 200000
static uint64_t primes[MAX_PRIMES];
static size_t primes_cnt = 0;

static uint8_t prime_prop[2000001] = {0};   /* 0=unknown,1=false,2=true */

static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    const uint64_t LIMIT = 2000000;
    bool *comp = (bool *)malloc(LIMIT + 1);
    memset(comp, 0, LIMIT + 1);
    primes_cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!comp[i]) {
            primes[primes_cnt++] = i;
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    comp[j] = true;
            }
        }
    }
    free(comp);
    done = true;
}

static bool is_primitive_root2_prime(uint64_t p)
{
    if (p <= 2) return false;
    uint64_t m = p - 1;
    uint64_t t = m;
    for (size_t i = 0; i < primes_cnt &&
                     (uint64_t)primes[i] * (uint64_t)primes[i] <= t; ++i) {
        uint64_t q = primes[i];
        if (t % q == 0) {
            while (t % q == 0) t /= q;
            if (mod_pow64(2, m / q, p) == 1)
                return false;               /* order divides (p-1)/q */
        }
    }
    if (t > 1) {
        if (mod_pow64(2, m / t, p) == 1)
            return false;
    }
    return true;
}

static bool is_primitive_root2_prime_cached(uint64_t p)
{
    if (p <= 2) return false;
    uint8_t v = prime_prop[p];
    if (v != 0) return v == 2;
    bool r = is_primitive_root2_prime(p);
    prime_prop[p] = r ? 2 : 1;
    return r;
}

/* ---------- test whether m is a power of a primitive‑root-2 prime ---------- */
static bool is_power_of_primitive_prime(uint64_t m)
{
    if (m <= 1) return false;
    uint64_t p = 0;
    uint64_t n = m;
    for (size_t i = 0; i < primes_cnt &&
                     (uint64_t)primes[i] * (uint64_t)primes[i] <= n; ++i) {
        uint64_t q = primes[i];
        if (n % q == 0) {
            if (p != 0) return false;   /* more than one distinct prime */
            p = q;
            while (n % q == 0) n /= q;
        }
    }
    if (n > 1) {
        if (p != 0) return false;
        p = n;
    }
    return is_primitive_root2_prime_cached(p);
}

/* ---------- main sequence generator ---------- */
seq_t a(idx_t n)
{
    /* Offset 1: a(1)=1, a(2)=2 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    init_primes();

    idx_t count = 2;          /* number of terms found (1 and 2) */
    uint64_t i = 3;           /* start checking from 3 */
    for (;;) {
        bool member = false;
        if (i & 1u) {        /* odd numbers – possible p^e */
            if (is_power_of_primitive_prime(i))
                member = true;
        } else {             /* even numbers – possible 2*p^e */
            uint64_t j = i >> 1;
            if ((j & 1u) && is_power_of_primitive_prime(j))
                member = true;
        }

        if (member) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
        ++i;
    }
    /* unreachable */
    return (seq_t)0;
}
