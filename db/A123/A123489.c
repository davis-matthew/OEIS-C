/*
 * A123489  a(n) = Sum_{k=0..p-1} Kronecker(4k^3+1, p) where p is the n-th
 *          prime of the form 6k+1.
 * https://oeis.org/A123489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..p-1} Kronecker(4k^3+1, p) where p is the n-th
 *            prime of the form 6k+1 (offset 1)
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Legendre (Jacobi) symbol, returns -1, 0, or +1 */
#include "../utils/jacobi_symbol.h"
/* primality test for idx_t values */
#include "../utils/is_prime.h"

/* Cache for primes of the form 6k+1 (1‑based indexing) */
static uint32_t *primes06 = NULL;   /* array of primes 6k+1 */
static size_t   primes06_cap = 0;   /* allocated capacity */
static size_t   primes06_cnt = 0;   /* number of primes stored */

/* Cache for computed a(n) values (1‑based indexing) */
static seq_t *sum_cache = NULL;    /* array of sums  */
static size_t  sum_cache_cap = 0;  /* allocated capacity */
static size_t  sum_cache_cnt = 0;  /* number of sums stored */

/* Make sure primes06 holds at least n primes of the form 6k+1 */
static void ensure_primes06(idx_t n)
{
    if (primes06_cnt >= n)
        return;

    idx_t start = (primes06_cnt == 0) ? 7 : primes06[primes06_cnt - 1] + 6;

    for (idx_t cand = start; primes06_cnt < n; cand += 6) {
        if (is_prime(cand)) {
            if (primes06_cnt == primes06_cap) {
                size_t new_cap = primes06_cap ? primes06_cap * 2 : 32;
                uint32_t *tmp = (uint32_t *)realloc(primes06,
                                                   new_cap * sizeof(uint32_t));
                if (!tmp) abort();
                primes06 = tmp;
                primes06_cap = new_cap;
            }
            primes06[primes06_cnt++] = (uint32_t)cand;
        }
    }
}

/* Return the n‑th prime of the form 6k+1 (1‑based) */
static uint32_t get_prime_of_form(idx_t n)
{
    ensure_primes06(n);
    return primes06[n - 1];
}

/* Compute a prime value p (idx_t) and return the Kronecker sum over k */
static seq_t compute_sum(idx_t p)
{
    seq_t sum = 0;
    for (idx_t k = 0; k < p; ++k) {
        /* compute (4*k^3 + 1) mod p safely using 128‑bit arithmetic for overflow */
        __int128 k128 = k;
        __int128 val   = (( (__int128)4 * k128 % p ) * k128 % p) * k128 % p;
        val = (val + 1) % p;
        int ls = jacobi_symbol((uint64_t)val, (uint64_t)p); /* Legendre symbol */
        sum += (seq_t)ls;
    }
    return sum;
}

/* Make sure sum_cache holds values up to index n (1‑based) */
static void ensure_sum_cache(idx_t n)
{
    if (sum_cache_cnt >= n)
        return;

    for (idx_t i = sum_cache_cnt + 1; i <= n; ++i) {
        uint32_t p = get_prime_of_form(i);
        seq_t val  = compute_sum((idx_t)p);

        if (sum_cache_cnt == sum_cache_cap) {
            size_t new_cap = sum_cache_cap ? sum_cache_cap * 2 : 32;
            seq_t *tmp = (seq_t *)realloc(sum_cache,
                                          new_cap * sizeof(seq_t));
            if (!tmp) abort();
            sum_cache = tmp;
            sum_cache_cap = new_cap;
        }
        sum_cache[sum_cache_cnt++] = val;
    }
}

/* Public function implementing the OEIS sequence */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset behavior is irrelevant */
        return 0;
    ensure_sum_cache(n);
    return sum_cache[n - 1];
}
