/*
 * A096278  Sums of successive sums of successive sums of successive primes.
 * https://oeis.org/A096278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) + 3*prime(n+1) + 3*prime(n+2) + prime(n+3)
 * Range:     u8 n <= 9, u16 n <= 1026, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* prime_cache holds primes found so far (1‑based indexing in the cache, 0‑based array) */
static idx_t *prime_cache = NULL;
static size_t   prime_cap   = 0;
static size_t   prime_cnt   = 0;
static idx_t    next_candidate = 2;   /* next integer to test for primality */

/* Return the k‑th prime (k ≥ 1).  Primes are generated lazily and cached. */
static seq_t get_prime(idx_t k)
{
    if (k == 0) return (seq_t)0;  /* safety; never used */

    while (prime_cnt < k) {
        idx_t n = next_candidate;
        bool is_prime = true;

        /* trial division by primes already known */
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_cache[i];
            if (p * p > n) break;
            if (n % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            if (prime_cnt == prime_cap) {   /* grow cache */
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(prime_cache, new_cap * sizeof(idx_t));
                if (!tmp) exit(EXIT_FAILURE);  /* allocation failed */
                prime_cache = tmp;
                prime_cap   = new_cap;
            }
            prime_cache[prime_cnt++] = n;   /* store newly found prime */
        }
        ++next_candidate;                  /* advance test candidate */
    }
    return (seq_t)prime_cache[k - 1];
}

/* External linkage as required */
seq_t a(idx_t n)
{
    seq_t p1 = get_prime(n);
    seq_t p2 = get_prime(n + 1);
    seq_t p3 = get_prime(n + 2);
    seq_t p4 = get_prime(n + 3);

    return p1 + ((seq_t)3 * p2) + ((seq_t)3 * p3) + p4;
}
