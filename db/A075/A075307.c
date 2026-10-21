/*
 * A075307  Smallest number m > 1 with m == k mod k-th prime for k = 1 to n.
 * https://oeis.org/A075307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m > 1 with m == k (mod p_k) for k = 1..n, where
 *            p_k is the k-th prime
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   mod_inverse_small, seq_t_mod_uint64, sieving_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: This uses a dynamic CRT construction.  The result is stored
   in a static cache so that calls can occur in arbitrary order.  It
   also builds a table of the first n primes on demand.  All arithmetic
   is performed using the seq_t type; modulo reductions against
   small primes are done via a helper that casts the modulus to seq_t. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../utils/mod_inverse_small.h"
#include "../utils/seq_t_mod_uint64.h"
#include "../utils/sieving_first_n_primes.h"

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n <= 0 */

    /* static state */
    static seq_t *cache = NULL;
    static size_t cache_used = 0;

    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static uint64_t prime_limit = 0;

    static seq_t cur_m;           /* current solution */
    static seq_t cur_prod;        /* product of primes processed */

    /* initialise on first call */
    if (cache == NULL) {
        cache_used = 1;
        cache = (seq_t *)calloc(2, sizeof(seq_t));  /* indices 0 and 1 */
        if (!cache) return 0;                 /* allocation failure */
        /* a(1) = 3 (smallest >1 with m ≡ 1 (mod 2)) */
        cache[1] = (seq_t)3;
        cur_m = cache[1];
        cur_prod = (seq_t)2;                 /* product of first prime */
        /* prepare at least one prime (2) */
        prime_cnt = 1;
        prime_list = (uint64_t *)malloc(sizeof(uint64_t));
        prime_list[0] = 2ULL;
        prime_limit = 2;
    }

    /* ensure we have the first n primes */
    if ((size_t)n > prime_cnt) {
        /* determine enough limit and resieve */
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        sieving_first_n_primes((idx_t)n, &new_primes, &new_cnt);
        if (!new_primes) return 0;
        /* free old list and recycle */
        free(prime_list);
        prime_list = new_primes;
        prime_cnt = new_cnt;
    }

    /* extend cache up to n */
    if ((size_t)n > cache_used) {
        /* grow cache */
        seq_t *new_cache = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!new_cache) { free(cache); return 0; }
        cache = new_cache;
        memset(cache + cache_used + 1, 0, ((size_t)n - cache_used) * sizeof(seq_t));

        /* advance from cache_used+1 to n */
        for (size_t i = cache_used + 1; i <= (size_t)n; ++i) {
            uint64_t p = prime_list[i - 1];          /* i-th prime (1‑based) */
            uint64_t cur_prod_mod = seq_t_mod_uint64(cur_prod, p);
            uint64_t inv = mod_inverse_small(cur_prod_mod, p);
            uint64_t cur_m_mod = seq_t_mod_uint64(cur_m, p);
            uint64_t rhs = ((uint64_t)i % p + p - cur_m_mod) % p;
            uint64_t t = (rhs * inv) % p;
            cur_m = cur_m + cur_prod * (seq_t)t;
            cur_prod = cur_prod * (seq_t)p;
            cache[i] = cur_m;
        }
        cache_used = (size_t)n;
    }

    return cache[(size_t)n];
}
