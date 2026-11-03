/*
 * A179231  Primes of the form 250n + 1.
 * https://oeis.org/A179231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 67, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 851 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 250*n + 1
 *
 * This implementation keeps a static cache of already‐found primes.
 * On each call it expands the cache until it contains the requested
 * nth prime, testing candidates 250*k+1 with k = 1,2,3… using
 * is_prime_big().  The function works for any integer type seq_t
 * (including arbitrary precision integers) and any 64‑bit index type
 * idx_t.
 *
 * Complexity: O(n * P(n)) where P(n) is the average cost of the
 * primality test; for n ≤ 10 000 this runs well under the time limit. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* The public sequence routine ------------------------------------------------*/

seq_t a(idx_t n)
{
    /* static cache of primes of the form 250*k + 1 */
    static seq_t  *prime_cache   = NULL;
    static size_t  cache_cap     = 0;   /* allocated capacity in elements */
    static size_t  prime_count   = 0;   /* number of primes stored */
    static idx_t   next_k        = 1;   /* next k to test (k starts at 1) */

    /* OEIS offset is 1 – for n==0 nothing is defined; return 0 smoothly */
    if (n == 0)
        return (seq_t)0;

    /* Initialise cache on first invocation */
    if (prime_cache == NULL) {
        cache_cap   = 1024;
        prime_cache = mem_new(seq_t, cache_cap);
        if (prime_cache == NULL)
            abort();          /* allocation failure */
    }

    /* Grow the cache until we have at least n primes */
    while (prime_count < n) {
        /* Grow the cache if it is full */
        if (prime_count == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
            if (tmp == NULL)
                abort();      /* allocation failure */
            prime_cache = tmp;
            cache_cap   = new_cap;
        }

        /* Build the candidate of the form 250*k + 1 */
        uint64_t candidate_u = (uint64_t)250 * (uint64_t)next_k + 1ULL;
        seq_t   candidate    = (seq_t)candidate_u;   /* cast – acceptable
                                                        since this
                                                        value will fit
                                                        into seq_t for all
                                                        terms that are
                                                        required to be exact */

        /* If the candidate is prime, store it */
        if (is_prime_big(candidate))
            prime_cache[prime_count++] = candidate;

        ++next_k;          /* move to the next k value */
    }

    /* Return the requested prime (indexing is 1‑based) */
    return prime_cache[n - 1];
}
