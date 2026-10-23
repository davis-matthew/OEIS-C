/*
 * A105884  Primes for which -12 is a primitive root.
 * https://oeis.org/A105884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = the n‑th prime p for which -12 is a primitive root modulo p.  
   The function generates primes on demand and tests each for the primitive
   root condition using the factorisation of φ(p)=p−1.  
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* fast modular exponentiation using 128‑bit multiplication */
static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return res;
}

/* prime storage */
static uint64_t *primes = NULL;
static size_t    prime_cnt = 0;
static size_t    prime_cap = 0;

/* primes for which -12 is a primitive root */
static seq_t *prop = NULL;
static size_t  prop_cnt = 0;
static size_t  prop_cap = 0;

static uint64_t next_candidate = 3;
static bool initialized = false;

/* ensure capacity for the prime array */
static void ensure_prime_cap(size_t need) {
    if (prime_cap < need) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
        primes = tmp;
        prime_cap = new_cap;
    }
}

/* ensure capacity for the property array */
static void ensure_prop_cap(size_t need) {
    if (prop_cap < need) {
        size_t new_cap = prop_cap ? prop_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t*)realloc(prop, new_cap * sizeof(seq_t));
        prop = tmp;
        prop_cap = new_cap;
    }
}

/* trial‑division primality test using the current prime list */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* return the next prime > the current candidate */
static uint64_t next_prime(void) {
    for (;;) {
        uint64_t cand = next_candidate;
        next_candidate += 2;          /* skip even numbers */
        if (is_prime_uint64(cand)) {
            ensure_prime_cap(prime_cnt + 1);
            primes[prime_cnt++] = cand;
            return cand;
        }
    }
}

/* main sequence function: a(n) */
seq_t a(idx_t n) {
    size_t target = (size_t)n;          /* assume n fits in size_t for indexing */

    /* initialise global state on the first call */
    if (!initialized) {
        initialized = true;

        /* prime list */
        prime_cap = 16;
        primes = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        if (!primes) exit(1);
        primes[prime_cnt++] = 2;          /* first prime */

        /* property list */
        prop_cap = 16;
        prop = (seq_t*)malloc(prop_cap * sizeof(seq_t));
        if (!prop) exit(1);
    }

    /* generate primes until we have at least 'n' with the property */
    while (prop_cnt < target) {
        uint64_t p = next_prime();        /* get next prime */

        if (p <= 3) continue;             /* 2 and 3 cannot have -12 as a primitive root */

        /* compute g = (-12) mod p safely for all p */
        uint64_t g = (p - (12 % p)) % p;  /* 0 < g < p for p > 3 */

        uint64_t phi = p - 1;

        /* factorise φ(p) into distinct prime factors */
        uint64_t fac[64];
        size_t fac_cnt = 0;
        uint64_t temp = phi;
        for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
            uint64_t q = primes[i];
            if (temp % q == 0) {
                fac[fac_cnt++] = q;
                while (temp % q == 0) temp /= q;
            }
        }
        if (temp > 1) fac[fac_cnt++] = temp;

        /* test the primitive root condition */
        bool ok = true;
        for (size_t i = 0; i < fac_cnt && ok; ++i) {
            uint64_t q = fac[i];
            if (modpow64(g, phi / q, p) == 1) ok = false;
        }

        if (ok) {
            ensure_prop_cap(prop_cnt + 1);
            prop[prop_cnt++] = (seq_t)p;
        }
    }

    return prop[target - 1];
}
