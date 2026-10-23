/*
 * A142005  Primes congruent to 1 mod 31.
 * https://oeis.org/A142005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 221, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 1 modulo 62
   (hence also 1 modulo 31).  The function uses a simple
   dynamic sieve that grows until it has at least n such
   primes.  This works for all n ≥ 1, independent of
   call order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the list of found primes. */
static uint64_t *class_primes = NULL;   /* primes ≡ 1 (mod 62) */
static size_t   class_prime_count  = 0; /* number of stored primes */
static idx_t    sieve_limit         = 0; /* current upper bound of sieve */

/* Extend the sieve until we have at least 'needed' primes
   in the residue class.  The sieve limit is doubled each
   round until enough primes are found.  */
static void extend_sieve(idx_t needed)
{
    if (needed <= class_prime_count) return;

    idx_t limit = sieve_limit ? sieve_limit : 1000; /* start modestly */

    while (class_prime_count < needed) {
        /* Double the limit for the next round. */
        limit *= 2;
        if (limit < sieve_limit) break;            /* overflow guard */

        /* Allocate working array for the sieve (bytes). */
        char *is_prime = (char *)malloc((size_t)(limit + 1));
        if (!is_prime) return;                     /* allocation failed */

        /* Mark all entries as prime initially. */
        for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        /* Sieve of Eratosthenes */
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (is_prime[(size_t)i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[(size_t)j] = 0;
            }
        }

        /* Prepare new storage if necessary. */
        size_t capacity = class_prime_count ? class_prime_count : 1024;
        uint64_t *new_list = (uint64_t *)realloc(class_primes,
                                                 capacity * sizeof(uint64_t));
        if (!new_list) {
            free(is_prime);
            return;
        }
        class_primes = new_list;

        /* Count primes in the residue class up to the new limit. */
        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (is_prime[(size_t)i] && (i % 62ULL) == 1ULL) {
                if (cnt == capacity) {
                    capacity *= 2;
                    uint64_t *tmp = (uint64_t *)realloc(class_primes,
                                                        capacity * sizeof(uint64_t));
                    if (!tmp) {
                        free(is_prime);
                        return;
                    }
                    class_primes = tmp;
                }
                class_primes[cnt++] = (uint64_t)i;
            }
        }

        free(is_prime);

        class_prime_count = cnt;
        sieve_limit       = limit;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n=0 */

    if (class_prime_count < (size_t)n)
        extend_sieve(n);

    /* n is 1‑based, array is 0‑based. */
    return (seq_t)class_primes[n - 1];
}
