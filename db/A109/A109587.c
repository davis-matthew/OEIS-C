/*
 * A109587  a(n)=[prime(n+2)-2*prime(n+1)-prime(n)]^(n+1), where prime(k) is
 *          the k-th prime.
 * https://oeis.org/A109587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [prime(n+2)-2*prime(n+1)-prime(n)]^(n+1)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   estimate_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/estimate_nth_prime.h"

/* Helper: compute primes up to 'bound' (inclusive). */
static void sieve_primes(uint64_t bound, uint64_t **primes_arr, size_t *cnt, size_t *cap)
{
    /* Allocate boolean array */
    bool *sieve = (bool*)calloc(bound + 1, sizeof(bool));
    if (!sieve) return; /* allocation failure, leave empty */

    sieve[0] = sieve[1] = true; /* mark non-prime */
    for (uint64_t i = 2; i * i <= bound; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = i * i; j <= bound; j += i)
                sieve[j] = true;
        }
    }

    /* Count primes and store */
    size_t size = 0;
    for (uint64_t i = 2; i <= bound; ++i) {
        if (!sieve[i]) {
            if (size == *cap) {
                *cap = (*cap == 0) ? 16 : (*cap << 1);
                *primes_arr = (uint64_t*)realloc(*primes_arr, (*cap) * sizeof(uint64_t));
            }
            (*primes_arr)[size++] = i;
        }
    }
    *cnt = size;
    free(sieve);
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static idx_t prime_list_size = 0;          /* current largest k with primes[k-1] = prime(k) computed */

    /* lazy initialization / expansion */
    if (primes == NULL) {
        /* compute bound for initial n */
        unsigned __int128 bound32 = (unsigned __int128)estimate_nth_prime((idx_t)n + 2) + 100;
        /* convert to uint64_t; bound is small */
        uint64_t bound = (uint64_t)bound32;
        sieve_primes(bound, &primes, &primes_cnt, &primes_cap);
        prime_list_size = primes_cnt;
    } else if ((size_t)(n + 2) > prime_list_size) {
        /* need more primes */
        /* compute new bound big enough for the larger index */
        idx_t needed = (idx_t)n + 2;
        unsigned __int128 bound32 = (unsigned __int128)estimate_nth_prime(needed) + 100;
        uint64_t bound = (uint64_t)bound32;
        /* free previous array */
        free(primes);
        primes = NULL;
        primes_cnt = 0;
        primes_cap = 0;
        sieve_primes(bound, &primes, &primes_cnt, &primes_cap);
        prime_list_size = primes_cnt;
    }

    /* Retrieve primes: primes array 0-based, corresponds to prime(1)=2 @[0] */
    seq_t p_n     = (seq_t)primes[(size_t)n - 1];
    seq_t p_n1    = (seq_t)primes[(size_t)n];
    seq_t p_n2    = (seq_t)primes[(size_t)n + 1];

    seq_t diff = p_n2 - (seq_t)2 * p_n1 - p_n;
    idx_t exp = n + 1;

    /* Compute diff^(exp) */
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= diff;

    return result;
}
