/*
 * A090090  a(n) = prime(n)*prime(n+3).
 * https://oeis.org/A090090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) * prime(n+3) (offset 1)
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Prime cache (stored as idx_t for efficiency) */
static idx_t *prime_cache = NULL;   /* array of primes (2,3,5,…) */
static size_t prime_cnt = 0;        /* how many primes we have */
static size_t prime_cap = 0;        /* allocated capacity */

/* --------------------------------------------------------------------- */
/* Grow the prime cache so that it can hold at least new_cap elements      */
static void grow_prime_cache(size_t new_cap)
{
    if (prime_cap >= new_cap) return;
    size_t cap = prime_cap ? prime_cap : 16;
    while (cap < new_cap) cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(prime_cache, cap * sizeof(idx_t));
    prime_cache = tmp;
    prime_cap  = cap;
}

/* --------------------------------------------------------------------- */
/* Fill the cache with at least needed primes (1‑based index).            */
static void ensure_primes(size_t needed)
{
    if (prime_cnt >= needed) return;

    /* initialise with the first prime if cache is empty */
    if (prime_cnt == 0) {
        grow_prime_cache(16);
        prime_cache[0] = 2;
        prime_cnt = 1;
    }

    idx_t candidate = prime_cache[prime_cnt - 1] + 1;
    while (prime_cnt < needed) {
        /* trial division using primes we already know */
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_cache[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            grow_prime_cache(prime_cnt + 1);
            prime_cache[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* --------------------------------------------------------------------- */
/* Return the n‑th term of A090090 (n≥1). The sequence is defined as */
 /* a(n) = prime(n) * prime(n+3).                                       */
seq_t a(idx_t n)
{
    /* We need primes at indices n and n+3 (1‑based). */
    idx_t need = n + 3;
    ensure_primes(need);

    seq_t p1 = (seq_t)prime_cache[n - 1];     /* prime(n)  */
    seq_t p2 = (seq_t)prime_cache[n + 2];     /* prime(n+3) */

    return p1 * p2;
}
