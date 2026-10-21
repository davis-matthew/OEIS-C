/*
 * A096281  Sums of successive twin primes of order 1.
 * https://oeis.org/A096281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096281: sums of successive twin primes of order 1
   a(n) = p_n + p_{n+1}   (n ≥ 1)
   where p_i is the i‑th prime in the infinite sequence
   3,5,5,7,11,13,17,19,29,31,41,43,...  (each twin pair contributes
   its two primes; overlapping primes are counted twice). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/* helpers for dynamic uint64_t arrays                                   */
/* --------------------------------------------------------------------- */
static void grow_uint64_array(uint64_t **arr, size_t *cnt, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 128 : *cap;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    *arr = tmp;
    *cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* state of the sequence                                                  */
/* --------------------------------------------------------------------- */
static bool initialized = false;

static uint8_t *is_prime = NULL;
static size_t sieve_limit = 6000000;          /* large enough for n ≤ 10000 */

static size_t primes_cnt  = 0, primes_cap  = 0;
static uint64_t *primes   = NULL;

static size_t twin_cnt    = 0, twin_cap    = 0;
static uint64_t *twin_primes = NULL;

/* --------------------------------------------------------------------- */
/* initialise sieve, primes, twin primes                               */
/* --------------------------------------------------------------------- */
static void init_sequence(void)
{
    if (initialized) return;

    /* sieve of Eratosthenes */
    is_prime = (uint8_t *)calloc(sieve_limit + 1, sizeof(uint8_t));
    if (!is_prime) abort();
    memset(is_prime + 2, 1, sieve_limit - 1);   /* mark 2..LIMIT as prime */

    for (uint64_t i = 2; i * i <= sieve_limit; ++i) {
        if (!is_prime[i]) continue;
        for (uint64_t j = i * i; j <= sieve_limit; j += i) is_prime[j] = 0;
    }

    /* collect all primes up to the limit in a vector */
    for (uint64_t i = 2; i <= sieve_limit; ++i) {
        if (is_prime[i]) {
            grow_uint64_array(&primes, &primes_cnt, &primes_cap, primes_cnt + 1);
            primes[primes_cnt++] = i;
        }
    }

    /* build the sequence of primes appearing in twin pairs
       (each twin pair contributes its two primes, overlapping primes
        are automatically counted twice). */
    for (size_t i = 0; i + 1 < primes_cnt; ++i) {
        if (primes[i + 1] - primes[i] == 2) {
            grow_uint64_array(&twin_primes, &twin_cnt, &twin_cap, twin_cnt + 2);
            twin_primes[twin_cnt++] = primes[i];
            twin_primes[twin_cnt++] = primes[i + 1];
        }
    }

    initialized = true;
}

/* --------------------------------------------------------------------- */
/* public function                                                        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* outside the offset */

    init_sequence();

    /* we need at least n+1 items to compute a(n) = p_n + p_{n+1} */
    if ((size_t)n + 1 > twin_cnt) return (seq_t)0;  /* out of pre‑computed range */

    uint64_t sum = twin_primes[(size_t)n - 1] + twin_primes[(size_t)n];
    return (seq_t)sum;
}
