/*
 * A181835  The sum of the primes <= n that are strongly prime to n.
 * https://oeis.org/A181835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p <= n, p prime, gcd(p,n)=1, (n-1) mod p ≠ 0} p
 * Range:     u8 n <= 44, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Stored primes and current sieve limit. */
static idx_t        sieve_limit = 0;
static uint64_t    *prime_list  = NULL;
static size_t      prime_count = 0;
static size_t      prime_cap   = 0;

/* Simple incremental sieve up to `limit` (inclusive).  Updates global
   prime_list and prime_count.  The sieve is performed from scratch
   each time the limit is increased, which is efficient enough for n ≤ 10 000. */
static void sieve_to(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* Boolean array marking composite numbers. */
    bool *composite = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!composite) exit(EXIT_FAILURE);          /* allocation failed */

    /* Classic Eratosthenes sieve. */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!composite[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                composite[j] = true;
        }
    }

    /* Count how many primes were found. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!composite[i]) ++cnt;

    /* Allocate or grow the prime list if needed. */
    if (cnt > prime_cap) {
        uint64_t *new_list = (uint64_t *)realloc(prime_list, cnt * sizeof(uint64_t));
        if (!new_list) {
            free(composite);
            exit(EXIT_FAILURE);
        }
        prime_list = new_list;
        prime_cap  = cnt;
    }

    /* Fill the prime list. */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!composite[i]) prime_list[idx++] = (uint64_t)i;

    prime_count = cnt;
    sieve_limit = limit;
    free(composite);
}

/* Sequence a(n): sum of primes ≤ n that are strongly prime to n. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    sieve_to(n);

    seq_t sum = (seq_t)0;
    idx_t n_minus_one = n - 1;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > n) break;

        /* gcd(p,n)=1  ⇔  n % p != 0 because p is prime. */
        if (n % p == 0) continue;

        /* p should not divide n-1. */
        if (n_minus_one % p == 0) continue;

        sum += (seq_t)p;
    }

    return sum;
}
