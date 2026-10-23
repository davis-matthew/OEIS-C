/*
 * A137311  Numbers k such that a type-5 Gaussian normal basis over GF(2^k)
 *          exists.
 * https://oeis.org/A137311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1415, u32 n <= 156, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd, is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th integer k such that
   p = 5*k + 1 is prime and gcd(k, (p-1)/ord₂(p)) = 1,
   where ord₂(p) is the multiplicative order of 2 modulo p. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"
#include "../utils/build_primes_upto.h"

/* Cache for previously generated terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Prime list for trial division */
static bool primes_initialized = false;
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* Helper: test whether a given k satisfies the condition */
static bool qualifies(uint64_t k)
{
    /* p = 5*k + 1 */
    seq_t p = (seq_t)5 * (seq_t)k + (seq_t)1;
    if (!is_prime_big(p))
        return false;

    /* factor k into distinct prime factors */
    uint64_t temp = k;
    uint64_t f[64];
    size_t nf = 0;

    for (size_t i = 0; i < prime_cnt &&
         (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= temp; ++i) {
        uint64_t pr = prime_list[i];
        if (temp % pr == 0) {
            f[nf++] = pr;
            while (temp % pr == 0)
                temp /= pr;
        }
    }
    if (temp > 1)
        f[nf++] = temp;

    /* Ensure factor 5 is present (it divides p-1) */
    bool has5 = false;
    for (size_t i = 0; i < nf; ++i)
        if (f[i] == 5) { has5 = true; break; }
    if (!has5)
        f[nf++] = 5;

    /* Compute ord₂(p) by reducing p-1 = 5*k */
    seq_t r   = (seq_t)5 * (seq_t)k;   /* p-1 */
    seq_t ord = r;                     /* current order candidate */

    for (size_t i = 0; i < nf; ++i) {
        uint64_t q = f[i];
        while (ord % (seq_t)q == 0) {
            uint64_t exp = (uint64_t)(ord / (seq_t)q);
            if (mod_pow((seq_t)2, exp, p) == (seq_t)1)
                ord = ord / (seq_t)q;
            else
                break;
        }
    }

    /* d = (p-1)/ord₂(p) */
    seq_t d = r / ord;

    /* gcd(k, d) == 1 ? */
    seq_t g = gcd((seq_t)k, d);
    return g == (seq_t)1;
}

seq_t a(idx_t n)
{
    /* Lazy prime list initialization */
    if (!primes_initialized) {
        const idx_t limit = 2000000ULL;        /* sufficient for k up to ~1e6 */
        build_primes_upto(limit, &prime_list, &prime_cnt);
        primes_initialized = true;
    }

    /* Generate terms until we have at least n terms */
    static uint64_t last_checked = 0;
    while (cache_len < n) {
        ++last_checked;
        if (qualifies(last_checked)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[(size_t)(n - 1)];
}
