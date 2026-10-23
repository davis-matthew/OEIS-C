/*
 * A107425  Numbers k such that (k + prime(k)) and (k+1 + prime(k+1)) are
 *          divisible by 7.
 * https://oeis.org/A107425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 2088, u32 n <= 9850, u64 n <= 9850,
 *            u128 n <= 9850, big any n
 * Helpers:   estimate_limit_for_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107425: Numbers k such that (k + prime(k)) and (k+1 + prime(k+1)) are divisible by 7. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../utils/estimate_limit_for_primes.h"

/*------------------------------------------------------------------------
 *  Prime generation helpers
 *------------------------------------------------------------------------*/
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t      prime_cnt  = 0;     /* number of primes stored */
static size_t      prime_cap  = 0;     /* capacity of prime_list */
static bool        primes_ready = false;

/* increase the capacity of prime_list */
static void grow_prime_cap(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) { free(prime_list); exit(1); }
    prime_list = tmp;
    prime_cap = new_cap;
}

/* build a new list of primes up to 'limit' (inclusive) using Eratosthenes */
static void build_primes_up_to(size_t limit)
{
    unsigned char *is_comp = (unsigned char *)malloc(limit + 1);
    if (!is_comp) exit(1);
    memset(is_comp, 0, limit + 1);
    if (limit >= 0) is_comp[0] = 1;
    if (limit >= 1) is_comp[1] = 1;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_comp[i]) continue;
        for (size_t j = i * i; j <= limit; j += i) is_comp[j] = 1;
    }

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (!is_comp[i]) ++cnt;

    grow_prime_cap(cnt);
    prime_cnt = cnt;
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) if (!is_comp[i]) {
        prime_list[idx++] = (uint64_t)i;
    }
    free(is_comp);
    primes_ready = true;
}

/* Ensure that prime_list contains at least 'idx' primes. */
static void ensure_primes_for_index(size_t idx)
{
    if (prime_cnt >= idx) return;
    /* compute required sieve limit and rebuild primes */
    size_t limit = estimate_limit_for_primes(idx);
    build_primes_up_to(limit);
}

/*------------------------------------------------------------------------
 *  Main sequence function
 *------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defined arbitrarily for n=0 */

    /* Find the nth term by scanning k */
    uint64_t k = 1;          /* current candidate k */
    size_t found = 0;        /* number of terms found so far */

    while (true) {
        /* Need prime(k) and prime(k+1) */
        ensure_primes_for_index((size_t)k + 1);

        uint64_t pk  = prime_list[(size_t)k - 1];
        uint64_t pk1 = prime_list[(size_t)k];

        if (((k + pk) % 7ULL == 0ULL) && (((k + 1ULL) + pk1) % 7ULL == 0ULL)) {
            ++found;
            if (found == (size_t)n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
