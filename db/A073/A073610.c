/*
 * A073610  Number of primes of the form n-p where p is a prime.
 * https://oeis.org/A073610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of ordered prime pairs (p,q) with p+q = n
 * Range:     u8 n <= 2729, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a sieve of Eratosthenes up to n on demand
   and counts pairs using the generated list of primes. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the framework */
extern seq_t a(idx_t n);

/* -------------------------------------------------------------- */

/* The function that returns the desired sequence value.  */
seq_t a(idx_t n)
{
    /* Static storage for the sieve and prime list. */
    static uint8_t *is_prime_arr = NULL;   /* 0 = composite, 1 = prime */
    static idx_t   sieve_limit   = 0;       /* highest number sieved */
    static uint64_t *prime_vec   = NULL;    /* list of primes */
    static size_t   prime_cnt     = 0;       /* count of primes */

    /* Re‑build if the requested n is larger than we have sieved. */
    if (n > sieve_limit) {
        /* Allocate a new sieve array.  Use (n+1) to hold indices 0..n. */
        uint8_t *new_arr = (uint8_t*)malloc((n + 1) * sizeof(uint8_t));
        if (!new_arr) return 0;                  /* out of memory – safe fallback */

        /* Initialise all entries to true (prime). */
        for (idx_t i = 0; i <= n; ++i) new_arr[i] = 1;
        new_arr[0] = new_arr[1] = 0;             /* 0 and 1 are not prime */

        /* Allocate a prime list big enough for n (worst case ~n). */
        uint64_t *new_primes = (uint64_t*)malloc((n + 1) * sizeof(uint64_t));
        size_t new_cnt = 0;

        /* Sieve of Eratosthenes. */
        for (idx_t i = 2; i <= n; ++i) {
            if (new_arr[i]) {                    /* i is prime */
                new_primes[new_cnt++] = i;
                /* Mark multiples of i as composite.  The loop
                   limits i*i to avoid overflow on small i. */
                if (i * (idx_t)i <= n) {
                    for (idx_t j = i * i; j <= n; j += i) new_arr[j] = 0;
                }
            }
        }

        /* Replace old data with the new arrays. */
        free(is_prime_arr);
        free(prime_vec);
        is_prime_arr = new_arr;
        prime_vec    = new_primes;
        prime_cnt    = new_cnt;
        sieve_limit  = n;
    }

    /* Count ordered prime pairs (p,q) with p+q=n. */
    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_vec[i];
        if (p > n) break;
        idx_t q = n - p;
        if (q < 2) continue;
        if (is_prime_arr[q]) ++count;   /* q is prime */
    }

    return count;
}
