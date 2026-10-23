/*
 * A103491  Multiplicative suborder of 5 (mod n) = sord(5, n).
 * https://oeis.org/A103491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 522, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64, lcm_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the multiplicative suborder of 5 modulo n.
   a(n) = min e>0 such that 5^e ≡ ±1 (mod n),
   or 0 if no such e exists.  */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd_uint64.h"
#include "../utils/lcm_uint64.h"

#define PRIME_LIMIT 100000   /* sufficient for factoring n ≤ 10 000 */

/* ---------- prime sieve (executed once) ---------- */
static uint32_t primes[PRIME_LIMIT];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0) return;            /* already initialized */
    static bool composite[PRIME_LIMIT];
    for (int i = 2; i < PRIME_LIMIT; ++i)
        if (!composite[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if ((int64_t)i * i < PRIME_LIMIT)
                for (int j = i * i; j < PRIME_LIMIT; j += i)
                    composite[j] = true;
        }
}

/* ---------- basic utilities ---------- */

/* fast modular exponentiation, 64‑bit operands */
static inline uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

/* factorise x into distinct primes into fac[0..cnt-1] */
static void factor_uint64(uint64_t x, uint64_t *fac, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        uint32_t p = primes[i];
        if (x % p == 0) {
            fac[(*cnt)++] = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) fac[(*cnt)++] = x;
}

/* order of 5 modulo a prime p (p ≠ 5) */
static uint64_t order_mod_prime(uint32_t p)
{
    uint64_t ord = p - 1;
    uint64_t fac[64];
    size_t fc;
    factor_uint64(ord, fac, &fc);
    for (size_t i = 0; i < fc; ++i) {
        uint64_t f = fac[i];
        while (ord % f == 0 && powmod_uint64(5, ord / f, p) == 1)
            ord /= f;
    }
    return ord;
}

/* order of 5 modulo p^k (p ≠ 5) */
static uint64_t order_mod_prime_power(uint32_t p, unsigned k)
{
    uint64_t mod = 1;
    for (unsigned i = 0; i < k; ++i) mod *= p;
    uint64_t ord = order_mod_prime(p);
    while (powmod_uint64(5, ord, mod) != 1) {
        ord *= p;
    }
    return ord;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n <= 1) return (seq_t)0;
    if (n % 5 == 0) return (seq_t)0;

    init_primes();

    uint64_t rem = n;
    uint64_t lcm_order = 1;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= rem; ++i) {
        uint32_t p = primes[i];
        if (rem % p == 0) {
            unsigned exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            uint64_t ord = order_mod_prime_power(p, exp);
            lcm_order = lcm_uint64(lcm_order, ord);
        }
    }
    if (rem > 1) { /* remaining prime factor */
        uint32_t p = (uint32_t)rem;
        uint64_t ord = order_mod_prime(p);
        lcm_order = lcm_uint64(lcm_order, ord);
    }

    uint64_t e = lcm_order;
    if (lcm_order % 2 == 0) {
        uint64_t half = lcm_order / 2;
        if (powmod_uint64(5, half, n) == n - 1)
            e = half;
    }

    return (seq_t)e;
}
