/*
 * A038888  Primes p such that 15 is not a square mod p.
 * https://oeis.org/A038888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A038888
    a(n) = the n-th prime p for which 15 is NOT a quadratic residue mod p
    (i.e. Legendre symbol (15|p) = -1, excluding p = 3 or 5). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

static seq_t *prime_list = NULL;    /* cached list of sequence terms   */
static size_t prime_cnt   = 0;      /* number of terms currently stored    */
static size_t prime_cap   = 0;      /* allocated capacity of prime_list */
static uint64_t last_prime = 2;     /* last prime returned by next_prime   */

/* small mod‑exponentiation, all operands fit in 64 bits */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1ULL;
    }
    return result;
}

/* guarantee capacity for at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (prime_cap < need) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 128;
        while (new_cap < need) new_cap *= 2;
        prime_list = (seq_t*)realloc(prime_list, new_cap * sizeof(seq_t));
        prime_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* memoise; generate until at least n terms are ready */
    while (prime_cnt < n) {
        uint64_t p = next_prime(last_prime);
        last_prime = p;

        /* skip trivial cases p | 15 */
        if (p == 3 || p == 5) continue;

        /* compute Legendre symbol (15|p) via Euler's criterion */
        uint64_t exp = (p - 1) / 2;
        uint64_t res = mod_pow_u64(15 % p, exp, p);

        /* Legendre symbol = -1 iff res == p-1 */
        if (res == p - 1) {
            ensure_capacity(prime_cnt + 1);
            prime_list[prime_cnt++] = (seq_t)p;
        }
    }
    return prime_list[n - 1];
}
