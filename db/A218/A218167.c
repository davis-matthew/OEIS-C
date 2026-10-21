/*
 * A218167  a(n) is the smallest positive integer k such that k^512 + 1 == 0
 *          mod p(n) where p(n) are the n-th prime of the form 1 + 1024*b.
 * https://oeis.org/A218167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 16, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by finding the n‑th prime of the form 1 + 1024·b
   and then determining the smallest positive k such that k^512 ≡ −1 (mod p). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/mod_pow.h"

/* Static cache of primes 1 + 1024*b and of computed terms */
static idx_t *primes_of_form = NULL;
static size_t primes_of_form_cnt = 0;

static seq_t *term_cache = NULL;
static size_t term_cache_cnt = 0;

/* Estimate a sieving limit that guarantees enough primes of the form 1+1024*b
   for n up to about 10000.  100 000 000 is safe. */
#define SIEVE_LIMIT   100000000ULL

/* Ensure global structures are initialized */
static void init_globals(void)
{
    if (primes_of_form != NULL)
        return;

    idx_t *all_primes;
    size_t all_cnt;
    build_primes(SIEVE_LIMIT, &all_primes, &all_cnt);

    /* Allocate maximum possible space; we will shrink later if desired */
    primes_of_form = (idx_t *)malloc(all_cnt * sizeof(idx_t));
    primes_of_form_cnt = 0;

    /* Keep only primes that are ≡ 1 mod 1024 */
    for (size_t i = 0; i < all_cnt; ++i)
    {
        if (((all_primes[i] - 1u) & 1023u) == 0u)
            primes_of_form[primes_of_form_cnt++] = all_primes[i];
    }
    free(all_primes);

    /* Allocate cache for terms */
    term_cache = (seq_t *)malloc(primes_of_form_cnt * sizeof(seq_t));
    term_cache_cnt = 0;
}

/* Compute the smallest k such that k^512 ≡ –1 (mod p),
   where p is prime and p ≡ 1 (mod 1024). */
static seq_t compute_term_for_prime(idx_t prime)
{
    uint64_t p = (uint64_t)prime;
    uint64_t N = p - 1;
    uint64_t q = N / 1024ULL;          /* (p-1)/1024 */

    /* Factorise N to get distinct prime factors. */
    uint64_t tmp = N;
    uint64_t facs[64];
    size_t nf = 0;

    if (tmp % 2 == 0) {
        facs[nf++] = 2;
        while (tmp % 2 == 0) tmp /= 2;
    }
    for (uint64_t d = 3; d * d <= tmp; d += 2) {
        if (tmp % d == 0) {
            facs[nf++] = d;
            while (tmp % d == 0) tmp /= d;
        }
    }
    if (tmp > 1) facs[nf++] = tmp;

    /* Find a primitive root g of p. */
    uint64_t g = 2;
    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < nf; ++i) {
            uint64_t f = facs[i];
            seq_t r = mod_pow((seq_t)g, N / f, (seq_t)p);
            if (r == 1) { ok = false; break; }
        }
        if (ok) break;
        ++g;
    }

    /* Compute base = g^q  and inc = g^(2q). */
    seq_t base = mod_pow((seq_t)g, q, (seq_t)p);
    seq_t inc  = mod_pow((seq_t)g, 2 * q, (seq_t)p);

    /* Enumerate the 512 solutions and keep the minimal. */
    seq_t best = base;
    seq_t cand = base;
    for (int i = 1; i < 512; ++i) {
        cand = (cand * inc) % (seq_t)p;
        if (cand < best) best = cand;
    }
    return best;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    init_globals();
    if (n == 0) return 0;              /* under‑offset; not used */

    /* Ensure that terms up to n are computed. */
    if (n > (idx_t)term_cache_cnt) {
        size_t up_to = (size_t)n;
        if (up_to > primes_of_form_cnt) {
            /* Should not happen for the supported range. */
            return 0;
        }
        for (size_t i = term_cache_cnt; i < up_to; ++i)
            term_cache[i] = compute_term_for_prime(primes_of_form[i]);
        term_cache_cnt = up_to;
    }
    return term_cache[(size_t)(n - 1)];
}
