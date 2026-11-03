/*
 * A128926  Smaller member p of a pair of consecutive primes (p,q) such that
 *          either q^2-p^2+1 or q^2-p^2-1 is also prime.
 * https://oeis.org/A128926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smaller prime p of a consecutive prime pair (p,q) such
 *            that * either q^2−p^2+1 or q^2−p^2−1 is also prime. *
 *            Computation: sequentially generate consecutive primes and test
 *            the condition, * caching results for arbitrary order of calls
 * Range:     u8 n <= 6, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Find the next prime greater than n using an inexpensive primality test. */
static uint64_t next_prime_uint64(uint64_t n)
{
    uint64_t cand = n + 1;
    while (!is_prime_seq((seq_t)cand))
        ++cand;
    return cand;
}

seq_t a(idx_t n)
{
    /* Static state: cache of found terms, prime list pointer, and previous prime. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t prev_prime = 2;   /* start with 2, so first pair is (2,3) */

    /* Initialise the cache once. */
    if (!cache)
    {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        uint64_t cur_prime = next_prime_uint64(prev_prime);

        /* Current pair (p,q). */
        seq_t p = (seq_t)prev_prime;
        seq_t q = (seq_t)cur_prime;

        /* Compute q^2 - p^2. */
        seq_t diff = q * q;
        diff -= p * p;          /* diff = q^2 - p^2, always positive */

        seq_t val1 = diff + (seq_t)1;
        seq_t val2 = diff - (seq_t)1;

        /* If either candidate is prime, store p. */
        if (is_prime_big(val1) || is_prime_big(val2))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                /* Basic error check – abort if allocation fails. */
                if (!tmp)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = p;
        }

        prev_prime = cur_prime;
    }

    return cache[n - 1];
}
