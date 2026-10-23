/*
 * A168048  a(n) = C(n)*Pi(n) where C(n) = number of nonprimes <= n, Pi(n) =
 *          number of primes <= n.
 * https://oeis.org/A168048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n)*Pi(n) where C(n) = number of non‑primes ≤ n, Pi(n) =
 *            number of primes ≤ n (counting 1 as non‑prime, 0 is not
 *            included)
 * Range:     u8 n <= 34, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static state for incremental sieve and caching of a(n) values */
    static seq_t *cache = NULL;            /* a(n) values, 0‑indexed (a(1) at cache[0]) */
    static size_t cache_size = 0;          /* number of cached a(n) values */

    static bool *is_comp = NULL;           /* composite flag for indices 0..M (M = current limit) */
    static size_t comp_size = 0;           /* allocated length of is_comp */

    static uint32_t *primes = NULL;        /* list of primes found so far */
    static size_t primes_size = 0;         /* number of primes stored */
    static size_t primes_cap   = 0;        /* allocated capacity of primes array */

    static seq_t *pi_cache = NULL;         /* Pi(i) for i = 0..M (M = current limit) */
    static size_t pi_size   = 0;           /* allocated length of pi_cache */

    if (n == 0)
        return 0;   /* undefined for n < 1, but return 0 for safety */

    /* Extend everything needed for index n */
    if ((idx_t)n > (idx_t)cache_size) {
        size_t need = (size_t)n;   /* we need cache[0 .. n-1] */

        /* ---- Extend sieve up to n ---- */
        if (need + 1 > comp_size) {
            size_t new_size = need + 1;   /* indices 0 .. n inclusive */
            bool *new_comp = (bool *)realloc(is_comp, new_size * sizeof(bool));
            if (!new_comp) abort();
            is_comp = new_comp;

            /* initialise fresh entries */
            for (size_t i = comp_size; i < new_size; ++i)
                is_comp[i] = false;

            /* sieve new entries */
            for (size_t i = comp_size; i < new_size; ++i) {
                if (i < 2) {
                    is_comp[i] = true;   /* 0 and 1 are not prime for our purposes */
                    continue;
                }
                bool is_prime = true;
                for (size_t j = 0; j < primes_size; ++j) {
                    uint32_t p = primes[j];
                    if ((uint64_t)p * (uint64_t)p > i) break;
                    if (i % p == 0) { is_prime = false; break; }
                }
                if (is_prime) {
                    /* store new prime */
                    if (primes_size == primes_cap) {
                        size_t new_cap = primes_cap ? primes_cap * 2 : 8;
                        uint32_t *new_arr = (uint32_t *)realloc(primes,
                                                               new_cap * sizeof(uint32_t));
                        if (!new_arr) abort();
                        primes = new_arr;
                        primes_cap = new_cap;
                    }
                    primes[primes_size++] = (uint32_t)i;
                } else {
                    is_comp[i] = true;
                }
            }
            comp_size = new_size;
        }

        /* ---- Extend Pi cache ---- */
        if (need + 1 > pi_size) {
            size_t new_pi_size = need + 1;
            seq_t *new_pi = (seq_t *)realloc(pi_cache, new_pi_size * sizeof(seq_t));
            if (!new_pi) abort();
            pi_cache = new_pi;

            seq_t count = (pi_size > 0) ? pi_cache[pi_size - 1] : 0;
            for (size_t i = pi_size; i < new_pi_size; ++i) {
                if (i < 2) {
                    pi_cache[i] = 0;
                    continue;
                }
                if (!is_comp[i]) ++count;
                pi_cache[i] = count;
            }
            pi_size = new_pi_size;
        }

        /* ---- Extend a(n) cache ---- */
        size_t new_cache_size = need;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cache_size * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        for (size_t i = cache_size; i < new_cache_size; ++i) {
            idx_t idx = (idx_t)(i + 1);                /* a(1) at cache[0] */
            seq_t pi_val  = pi_cache[idx];
            seq_t nonprime = (seq_t)idx - pi_val;      /* non‑primes ≤ idx */
            cache[i] = nonprime * pi_val;
        }
        cache_size = new_cache_size;
    }

    return cache[(size_t)n - 1];
}
