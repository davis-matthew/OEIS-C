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
 * Helpers:   generate_twin_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/generate_twin_primes.h"

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
        free(twin_vals);                          /* free old storage */
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
