/*
 * A135732  Distances to next prime associated with A135731.
 * https://oeis.org/A135732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=2; for n>=2, a(n) = p_{n+2} - p_{n+1} - 1 (p_k denotes the
 *            k‑th prime. The implementation builds the prime list on demand
 *            and computes the required distances.)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   estimate_limit_for_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/estimate_limit_for_index.h"

/* Static storage for the list of primes (0‑based): prime_list[0] = 2, etc. */
static idx_t *prime_list = NULL;        /* array of primes */
static size_t prime_cap  = 0;           /* allocated capacity of prime_list   */
static size_t prime_cnt  = 0;           /* number of primes stored            */

/* ------------------------------------------------------------------ */
/* Grow the prime_list array to have at least new_cap elements.       */
static void grow_prime_list(size_t new_cap)
{
    if (new_cap <= prime_cap) return;
    idx_t *new_list = (idx_t*)realloc(prime_list, new_cap * sizeof(idx_t));
    if (!new_list) exit(EXIT_FAILURE);   /* abort on allocation failure */
    prime_list = new_list;
    prime_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/* Sieve of Eratosthenes up to 'limit' (inclusive).  The primes are
   stored in prime_list; prime_cnt is updated.                          */
static void sieve_primes(idx_t limit)
{
    unsigned char *is_prime = (unsigned char*)malloc((size_t)(limit + 1));
    if (!is_prime) exit(EXIT_FAILURE);

    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

    /* Ensure storage and copy primes */
    grow_prime_list(cnt);
    prime_cnt = cnt;
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[idx++] = i;

    free(is_prime);
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Ensure we have at least 'required_index' primes in prime_list
   (i.e., primes[0] ... primes[required_index-1] exist). */
static void ensure_primes(size_t required_index)
{
    if (prime_cnt >= required_index) return;

    idx_t limit = estimate_limit_for_index(required_index + 1);

    /* Repeatedly sieve, doubling the limit until we have enough primes */
    while (prime_cnt < required_index) {
        sieve_primes(limit);
        if (prime_cnt >= required_index) return;
        limit *= 2;    /* widen the search */
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* Protected against undefined n=0 */

    if (n == 1) {
        /* Need primes p1 and p3: indices 0 and 2 in 0‑based array */
        ensure_primes(3);
        seq_t res = (seq_t)(prime_list[2] - ((seq_t)prime_list[0] + 1));
        return res;
    }

    /* For n >= 2: need primes p_{n+1} and p_{n+2} -> indices n and n+1 */
    ensure_primes((size_t)(n + 2));
    seq_t res = (seq_t)(prime_list[n + 1] - prime_list[n] - 1);
    return res;
}
