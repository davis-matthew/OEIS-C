/*
 * A101340  a(n) = p^p + p, with p = prime(n).
 * https://oeis.org/A101340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p^p + p, where p = prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow_seq.h"

/* Number of primes that fit in the sieve (enough for n <= 20000) */
#define MAX_PRIME_COUNT 20000
#define SIEVE_LIMIT      200000   /* > 2*10^5 > 20000th prime */

/* storage for generated primes */
static idx_t primes[MAX_PRIME_COUNT];
static idx_t prime_count = 0;
static bool prime_table_inited = false;

/* simple sieve of Eratosthenes to fill 'primes' array */
static void init_prime_table(void)
{
    if (prime_table_inited)
        return;

    static unsigned char sieve[SIEVE_LIMIT + 1];
    memset(sieve, 0, sizeof(sieve));

    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = 1;
        }
    }

    for (idx_t i = 2; i <= SIEVE_LIMIT && prime_count < MAX_PRIME_COUNT; ++i) {
        if (!sieve[i])
            primes[prime_count++] = i;
    }

    prime_table_inited = true;
}

/* compute a(n) = p^p + p where p = prime(n) */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* sequence is defined for n ≥ 1; return 0 for safety outside offset */
        return 0;
    }

    init_prime_table();

    /* For safety: if n is larger than precomputed primes, extend the sieve
       lazily (this path should not be hit for typical n ≤ 10000). */
    if (n > prime_count) {
        idx_t current_limit = SIEVE_LIMIT;
        unsigned char *s = NULL;
        while (prime_count < n) {
            current_limit *= 2;
            s = (unsigned char*)calloc(current_limit + 1, 1);
            if (!s) exit(EXIT_FAILURE);            /* allocation failed */
            /* perform sieve on new range */
            for (idx_t i = 2; i * i <= current_limit; ++i) {
                if (!s[i]) {
                    for (idx_t j = i * i; j <= current_limit; j += i)
                        s[j] = 1;
                }
            }
            for (idx_t i = 2; i <= current_limit && prime_count < n; ++i) {
                if (!s[i])
                    primes[prime_count++] = i;
            }
            free(s);
            s = NULL;
            if (current_limit > (1ULL << 31)) { /* avoid infinite loop on overflow */
                break;
            }
        }
    }

    seq_t p = primes[n - 1];             /* p = prime(n) */
    seq_t res = pow_seq(p, (idx_t)p);    /* p^p  (exponent fits in idx_t) */
    res += p;                            /* + p */
    return res;
}
