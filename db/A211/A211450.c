/*
 * A211450  (p-1)/x, where p = prime(n) and x = ord(5,p), the smallest
 *          positive integer such that 5^x == 1 mod p.
 * https://oeis.org/A211450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-1)/ord_5(prime(n)), with a(3)=0 because
 *            prime(3)=5 divides the base 5
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility functions provided by the build system */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

/* ---------- prime cache ---------- */

static uint32_t *prime_cache   = NULL;   /* array of primes */
static size_t   prime_cache_cnt = 0;     /* number of primes stored */

/* ensure that at least n primes are available in prime_cache */
static void
ensure_primes(idx_t n)
{
    if (n <= prime_cache_cnt)
        return;  /* already have enough */

    /* allocate or grow the array */
    uint32_t *new_primes = NULL;
    if (prime_cache == NULL) {
        new_primes = (uint32_t *)malloc(sizeof(uint32_t) * n);
    } else {
        new_primes = (uint32_t *)realloc(prime_cache, sizeof(uint32_t) * n);
    }
    if (!new_primes)  /* allocation failed – keep old cache and bail */
        return;

    prime_cache = new_primes;

    /* generate primes from prime_cache_cnt onwards */
    uint64_t candidate = (prime_cache_cnt == 0) ? 2U : (uint64_t)prime_cache[prime_cache_cnt - 1] + 1U;
    size_t   pos       = prime_cache_cnt;

    while (pos < n) {
        if (is_prime(candidate)) {
            prime_cache[pos++] = (uint32_t)candidate;
        }
        ++candidate;
    }

    prime_cache_cnt = n;
}

/* ---------- multiplicative order of 5 modulo prime p ---------- */

static uint64_t
ord5(uint64_t p)
{
    uint64_t ord = p - 1;          /* start with the full group order */
    uint64_t rem = ord;

    /* factor ord using prime_cache primes */
    for (size_t i = 0; i < prime_cache_cnt && (uint64_t)prime_cache[i] * (uint64_t)prime_cache[i] <= rem; ++i) {
        uint64_t q = prime_cache[i];
        if (rem % q != 0)
            continue;
        while (rem % q == 0)
            rem /= q;

        /* try to reduce the order by successive powers of q */
        while (ord % q == 0) {
            uint64_t candidate = ord / q;
            seq_t    pow_mod   = mod_pow((seq_t)5, candidate, (seq_t)p);
            if (pow_mod == (seq_t)1)
                ord = candidate;
            else
                break;
        }
    }

    /* if rem > 1, it is a prime factor larger than sqrt(orig ord) */
    if (rem > 1) {
        while (ord % rem == 0) {
            uint64_t candidate = ord / rem;
            seq_t    pow_mod   = mod_pow((seq_t)5, candidate, (seq_t)p);
            if (pow_mod == (seq_t)1)
                ord = candidate;
            else
                break;
        }
    }

    return ord;
}

/* ---------- main sequence function ---------- */

seq_t
a(idx_t n)
{
    if (n == 0)                /* undefined; return 0 safely */
        return (seq_t)0;

    /* obtain nth prime (1‑based) */
    ensure_primes(n);
    uint64_t p = (uint64_t)prime_cache[n - 1];

    if (p == 5)                /* special case */
        return (seq_t)0;

    uint64_t ord = ord5(p);

    /* (p - 1) / ord is always an integer */
    uint64_t temp = (p - 1) / ord;
    seq_t result = (seq_t)temp;
    return result;
}
