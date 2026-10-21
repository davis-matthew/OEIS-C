/*
 * A214643  Primes p such that p XOR 22 = p + 22.
 * https://oeis.org/A214643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Subsequence of primes satisfying (p ^ 22) == (p + 22) */
/* i.e., primes p with p & 22 == 0, equivalently p ≡ 1 or 9 (mod 32).  */
/* The function computes a(n) for n >= 1, caching results for arbitrary order of calls. */
#include <stdlib.h>
#include <stdint.h>

static seq_t *prime_cache = NULL;      /* cache of sequence terms */
static size_t prime_cache_cap = 0;
static size_t prime_cache_cnt = 0;

static seq_t *prime_list = NULL;       /* list of all primes seen */
static size_t prime_list_cap = 0;
static size_t prime_list_cnt = 0;

/* Append value to prime_cache, expanding if necessary */
static void cache_push(seq_t val) {
    if (prime_cache_cnt == prime_cache_cap) {
        size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure – terminate */
        prime_cache = tmp;
        prime_cache_cap = new_cap;
    }
    prime_cache[prime_cache_cnt++] = val;
}

/* Append prime to prime_list, expanding if necessary */
static void prime_push(seq_t val) {
    if (prime_list_cnt == prime_list_cap) {
        size_t new_cap = prime_list_cap ? prime_list_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_list_cap = new_cap;
    }
    prime_list[prime_list_cnt++] = val;
}

/* Test whether candidate is prime by trial division using already found primes. */
static int is_prime(seq_t cand) {
    if (cand < 2) return 0;
    for (size_t i = 0; i < prime_list_cnt; ++i) {
        seq_t p = prime_list[i];
        /* stop if p*p > cand */
        if (p * p > cand) break;
        if (cand % p == 0) return 0;
    }
    return 1;
}

/* Compute a(n) */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* behaviour for out‑of‑range requests */
    /* Return cached value if already computed */
    if ((size_t)n <= prime_cache_cnt) {
        return prime_cache[(size_t)n - 1];
    }

    /* If no primes have been generated yet, start with candidate 2 */
    seq_t candidate = prime_list_cnt ? prime_list[prime_list_cnt - 1] + 1 : 2;

    /* Generate primes and collect valid ones until we have n terms */
    while (prime_cache_cnt < (size_t)n) {
        /* Find next prime */
        while (!is_prime(candidate)) {
            ++candidate;
        }
        prime_push(candidate);                /* store prime */

        /* Check property: p ^ 22 == p + 22 */
        if ((candidate ^ (seq_t)22) == candidate + (seq_t)22) {
            cache_push(candidate);
        }
        ++candidate;   /* move to next candidate for next iteration */
    }

    return prime_cache[(size_t)n - 1];
}
