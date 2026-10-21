/*
 * A075068  Product of prime(n) primes starting from prime(n).
 * https://oeis.org/A075068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula: a(n) = product of the primes p_i for n ≤ i ≤ n−1+prime(n)
    (prime(1)=2, prime(2)=3, …).  The sequence begins: 6, 105, 85085, …  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Cached list of primes: 1‑based indexing in the definition, 0‑based in
   the array.  The array grows as needed. */
static size_t primes_cap    = 0;
static uint64_t *prime_arr  = NULL;
static size_t primes_cnt    = 0;
static uint64_t next_candidate = 2;          /* next number to test for primality */

/* Ensure that the array contains at least 'need' primes.
   The function grows the array and continues the sieve until
   primes_cnt == need. */
static void ensure_primes(size_t need)
{
    if (need <= primes_cnt)
        return;

    /* Grow the storage if required. */
    if (primes_cap < need) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 64;
        while (new_cap < need)
            new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
        if (!tmp)
            abort();                     /* allocation failure */
        prime_arr = tmp;
        primes_cap = new_cap;
    }

    /* Extend the sieve until the required number of primes is reached. */
    while (primes_cnt < need) {
        uint64_t cand = next_candidate;
        bool is_prime = true;
        /* Test up to sqrt(cand).  Use double for sqrt then cast back. */
        uint64_t limit = (uint64_t) sqrt((double) cand);

        for (size_t i = 0; i < primes_cnt && prime_arr[i] <= limit; ++i) {
            if (cand % prime_arr[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
            prime_arr[primes_cnt++] = cand;

        /* Advance to the next candidate.  After 2 we skip even numbers. */
        next_candidate = (cand == 2) ? 3 : cand + 2;
    }
}

/* Main sequence function.  Defined with external linkage. */
seq_t a(idx_t n)
{
    /* The offset is 1; behaviour for n==0 is not specified but we return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Convert n (uint64_t) to size_t safely – the problem limits n to ≈10000. */
    size_t n_idx = (size_t)n;

    /* Get prime(n). */
    ensure_primes(n_idx);
    uint64_t prime_n = prime_arr[n_idx - 1];          /* prime(n) */

    /* We need primes up to index n - 1 + prime(n). */
    size_t last_index = n_idx - 1 + prime_n;          /* inclusive index */
    ensure_primes(last_index);

    /* Multiply the required primes. */
    seq_t prod = (seq_t)1;
    for (size_t i = n_idx - 1; i < last_index; ++i)
        prod = prod * (seq_t)prime_arr[i];

    return prod;
}
