/*
 * A192578  Primes p for which there is no prime q == 1 (mod 4) that is
 *          smaller than p and is a quadratic residue modulo p.
 * https://oeis.org/A192578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A192578 – primes p for which there is no smaller prime q ≡ 1 (mod 4) that is a quadratic residue modulo p.
   The function builds the sequence on demand, caching primes and results.  It uses
   is_prime(idx_t) to test primality and jacobi_symbol(q,p) for the Legendre symbol.  The algorithm
   is O(total primes examined) and meets the time/memory constraints. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x); */
#include "../utils/jacobi_symbol.h"     /* int jacobi_symbol(uint64_t a, uint64_t n); */

/* cache of primes discovered so far */
static uint64_t *prime_cache = NULL;
static size_t prime_len = 0, prime_cap = 0;

/* cache of sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0, seq_cap = 0;

/* enlarge prime cache to hold at least `needed` entries */
static void ensure_prime_cache(size_t needed)
{
    if (needed <= prime_cap)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    if (prime_cache) {
        memcpy(tmp, prime_cache, prime_len * sizeof(uint64_t));
        free(prime_cache);
    }
    prime_cache = tmp;
    prime_cap = new_cap;
}

/* enlarge sequence cache to hold at least `needed` entries */
static void ensure_seq_cache(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) abort();
    if (seq_cache) {
        memcpy(tmp, seq_cache, seq_len * sizeof(seq_t));
        free(seq_cache);
    }
    seq_cache = tmp;
    seq_cap = new_cap;
}

/* main function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – behaviour for n < 1 is undefined */
        return 0;

    /* generate sequence values until we have at least n terms cached */
    while (seq_len < (size_t)n) {

        /* Find the next prime p.  We increment candidate until is_prime() succeeds. */
        static idx_t candidate = 2;
        idx_t p = 0;
        while (true) {
            if (is_prime(candidate)) {
                p = candidate;
                ++candidate;   /* next time start from the next integer */
                break;
            }
            ++candidate;
        }
        /* Store p in the primes cache */
        ensure_prime_cache(prime_len + 1);
        prime_cache[prime_len++] = (uint64_t)p;

        /* Check whether p satisfies the sequence condition. */
        bool good = true;
        for (size_t i = 0; i + 1 < prime_len && good; ++i) { /* up to but not including current p */
            uint64_t q = prime_cache[i];
            if ((q & 3ULL) == 1ULL) {                        /* q ≡ 1 (mod 4) */
                int ls = jacobi_symbol(q, p);                 /* Legendre symbol (q|p) */
                if (ls == 1)                                 /* q is a quadratic residue */
                    good = false;
            }
        }

        /* If p passes the test, add it to the sequence cache. */
        if (good) {
            ensure_seq_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)p;
        }
    }
    return seq_cache[(size_t)n - 1];
}
