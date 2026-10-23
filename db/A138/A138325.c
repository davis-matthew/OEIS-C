/*
 * A138325  Least prime p such that n = p + g, where g is a primitive root of
 *          p, or 0 if there is no such prime p.
 * https://oeis.org/A138325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that n = p + g, where g is a primitive
 *            root of p, * or 0 if no such prime exists
 * Range:     u8 n <= 458, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds primes on demand and tests each candidate prime
 * for the required property.  It works for any seq_t that can hold up to n
 * (e.g. uint64_t or an arbitrary‑precision type). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* prime array (dynamic) */
static uint64_t *primes_arr = NULL;
static size_t   primes_cnt = 0;
static size_t   primes_cap = 0;

/* ensure primes are generated up to at least 'limit' */
static void ensure_primes(uint64_t limit)
{
    if (primes_arr == NULL) {
        primes_cap = 64;
        primes_arr = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes_arr) return;          /* allocation failure: caller will return 0 */
        primes_cnt = 0;
        primes_arr[primes_cnt++] = 2;     /* first prime */
    }
    if (limit <= primes_arr[primes_cnt - 1]) return;

    for (uint64_t cand = primes_arr[primes_cnt - 1] + 1; cand <= limit; ++cand) {
        bool isp = true;
        for (size_t i = 0; i < primes_cnt && primes_arr[i] * primes_arr[i] <= cand; ++i) {
            if (cand % primes_arr[i] == 0) { isp = false; break; }
        }
        if (isp) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes_arr = (uint64_t *)realloc(primes_arr, primes_cap * sizeof(uint64_t));
                if (!primes_arr) return;  /* allocation failure */
            }
            primes_arr[primes_cnt++] = cand;
        }
    }
}

/* modular exponentiation (uint64_t) */
static uint64_t powmod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) res = mod_mul(res, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* test whether g is a primitive root modulo prime p */
static bool is_primitive(uint64_t g, uint64_t p)
{
    if (p == 2ULL) return g == 1ULL;
    if (g < 2ULL || g >= p) return false;

    uint64_t m   = p - 1;
    uint64_t rem = m;

    /* iterate over prime divisors of m */
    for (size_t i = 0; i < primes_cnt && primes_arr[i] * primes_arr[i] <= rem; ++i) {
        uint64_t q = primes_arr[i];
        if (rem % q == 0) {
            if (powmod(g, m / q, p) == 1ULL) return false;
            while (rem % q == 0) rem /= q;
        }
    }
    if (rem > 1 && powmod(g, m / rem, p) == 1ULL) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)0;   /* only n=3 gives a(3)=2, handled below */

    /* Candidate primes are at most n-1, but only those > n/2 may give g<p. */
    uint64_t limit = n - 1;
    ensure_primes(limit);

    /* find first prime >= ceil((n+1)/2) */
    uint64_t min_p = (n >> 1) + 1;          /* ceil((n+1)/2) */
    size_t i = 0;
    while (i < primes_cnt && primes_arr[i] < min_p) ++i;

    for (; i < primes_cnt && primes_arr[i] <= limit; ++i) {
        uint64_t p = primes_arr[i];
        uint64_t g = n - p;
        if (g >= p) continue;              /* g must be less than p to be a primitive root */
        if (is_primitive(g, p)) return (seq_t)p;
    }
    return (seq_t)0;
}
