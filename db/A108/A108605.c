/*
 * A108605  Semiprimes with prime sum of factors: twice the lesser of the twin
 *          prime pairs.
 * https://oeis.org/A108605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*p where p and p+2 are twin primes
 * Range:     u8 n <= 10, u16 n <= 505, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9702 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Generate twin primes up to 'lim', storing 2*p in *arr and count in *cnt. */
static void generate_twin_primes(idx_t lim, seq_t **arr, size_t *cnt)
{
    /* Allocate and initialise the prime sieve. */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)lim + 1);
    if (!is_prime) abort();
    memset(is_prime, 1, (size_t)lim + 1);
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= lim; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= lim; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count the twin primes to allocate the result array. */
    size_t tmp_cnt = 0;
    for (idx_t p = 2; p + 2 <= lim; ++p) {
        if (is_prime[p] && is_prime[p + 2]) {
            ++tmp_cnt;
        }
    }

    *arr = mem_new(seq_t, tmp_cnt);
    if (!*arr) abort();
    *cnt = tmp_cnt;

    /* Store the doubled smaller prime of each twin pair. */
    size_t pos = 0;
    for (idx_t p = 2; p + 2 <= lim; ++p) {
        if (is_prime[p] && is_prime[p + 2]) {
            seq_t val = (seq_t)p;
            val <<= 1;                /* multiply by 2 */
            (*arr)[pos++] = val;
        }
    }

    free(is_prime);
}

/* Initial sieve limit that covers most queries up to about n=10000. */
static const idx_t INITIAL_LIMIT = 2000000;

/* Static state */
static idx_t  sieve_limit = 0;          /* current sieve limit */
static idx_t  twin_cnt    = 0;          /* number of twin primes found */
static seq_t *twin_vals   = NULL;       /* array of 2*p values */

/* Ensure that we have at least 'n' elements in twin_vals. */
static void ensure_n(idx_t n)
{
    if (n <= twin_cnt) return;

    if (sieve_limit == 0) sieve_limit = INITIAL_LIMIT;
    while (n > twin_cnt) {
        if (sieve_limit > SIZE_MAX / 2) abort();   /* avoid overflow */
        sieve_limit *= 2;                         /* enlarge the sieve */
        seq_t *new_arr = NULL;
        size_t new_cnt = 0;
        generate_twin_primes(sieve_limit, &new_arr, &new_cnt);
        mem_free(twin_vals);                          /* free old storage */
        twin_vals = new_arr;
        twin_cnt  = new_cnt;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */
    ensure_n(n);
    return twin_vals[n - 1];       /* 1‑based indexing */
}
