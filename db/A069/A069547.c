/*
 * A069547  a(n) = n^2 mod n-th prime.
 * https://oeis.org/A069547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 mod p_n (p_n = the n‑th prime, offset 1)
 * Range:     u8 n <= 15, u16 n <= 1705, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   estimate_prime_interval
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This implementation keeps a dynamically resizable array of primes and
 * generates them on demand with a simple Eratosthenes sieve.  The array
 * is extended only when more primes are required, so repeated calls to
 * a(n) are fast even if the indices arrive in any order.
 */

#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/estimate_prime_interval.h"

/* dynamic array of primes (type idx_t) */
static idx_t  *prime_list  = NULL;     /* array of generated primes   */
static size_t  prime_cap   = 0;        /* current capacity in elements*/
static size_t  prime_cnt   = 0;        /* number of primes stored    */
static idx_t   sieve_limit = 0;        /* largest value computed by sieve */

/* ------------------------------------------------------------------
 * Ensure the prime array can hold at least `min_cap` elements.
 * ------------------------------------------------------------------ */
static void grow_prime_array(size_t min_cap)
{
    if (prime_cap >= min_cap) return;
    size_t new_cap = (prime_cap == 0) ? 64 : prime_cap * 2;
    while (new_cap < min_cap) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    if (!tmp) abort();                     /* out of memory */
    prime_list  = tmp;
    prime_cap   = new_cap;
}

/* ------------------------------------------------------------------
 * Sieve of Eratosthenes up to `lim`.  Overwrites the current prime
 * list with the primes in [2 … lim], resetting `prime_cnt` and
 * `sieve_limit`.
 * ------------------------------------------------------------------ */
static void sieve_upto(idx_t lim)
{
    if (lim <= sieve_limit) return;      /* already covered */

    /* allocate a boolean array: 0 → composite, 1 → prime */
    char *is_prime = (char *)calloc((size_t)lim + 1, sizeof(char));
    if (!is_prime) abort();              /* out of memory */
    for (idx_t i = 2; i <= lim; ++i) is_prime[i] = 1;

    idx_t sqrt_lim = (idx_t)floor(sqrt((double)lim));
    for (idx_t i = 2; i <= sqrt_lim; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= lim; j += i)
                is_prime[j] = 0;

    /* estimate how many primes we will have (rough bound) */
    size_t est_cnt = (size_t)(lim / log((double)lim) + 10);
    grow_prime_array(est_cnt);

    /* copy primes into the array */
    size_t count = 0;
    for (idx_t i = 2; i <= lim; ++i)
        if (is_prime[i]) {
            if (count >= prime_cap) grow_prime_array(prime_cap * 2);
            prime_list[count++] = i;
        }
    free(is_prime);
    prime_cnt   = count;
    sieve_limit = lim;
}

/* ------------------------------------------------------------------
 * Ensure that at least the first `n` primes are available.
 * ------------------------------------------------------------------ */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;
    idx_t limit = estimate_prime_interval(n);
    sieve_upto(limit);
    while (prime_cnt < (size_t)n) {
        limit <<= 1;   /* double the range and sieve again */
        sieve_upto(limit);
    }
}

/* ------------------------------------------------------------------
 * Sequence function: a(n) = n^2 mod p_n
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined in OEIS, but safe */
    ensure_primes(n);

    seq_t prime = (seq_t)prime_list[n - 1];   /* p_n (offset 1)  */
    seq_t sq    = (seq_t)n * (seq_t)n;        /* n^2 in seq_t    */

    return sq % prime;
}
