/*
 * A092758  Duplicate of A030516.
 * https://oeis.org/A092758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (prime(n))^6 where prime(n) is the nth prime (prime(1)=2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static storage for prime numbers (0-indexed: primes[0] == 2) */
static uint32_t *primes = NULL;      /* array of primes */
static size_t primes_cnt = 0;        /* number of stored primes */
static size_t primes_cap = 0;        /* capacity of the array */
static uint32_t last_checked = 1;    /* last integer checked for primality */

/* Ensure that the array contains at least 'need' primes */
static void ensure_primes(idx_t need)
{
    if (need == 0) return;
    size_t target = (size_t)need;

    /* Initialize the prime array with the first prime if not yet done */
    if (primes_cnt == 0) {
        primes_cap = 64;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) exit(1);          /* abort on allocation failure */
        primes[primes_cnt++] = 2;
        last_checked = 2;
    }

    /* Generate more primes until we have 'target' of them */
    while (primes_cnt < target) {
        uint32_t candidate = ++last_checked;

        /* Test candidate for primality */
        int is_prime = 1;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            /* stop if p * p > candidate (to avoid overflow use uint64_t) */
            if ((uint64_t)p * (uint64_t)p > candidate) break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* grow the array if necessary */
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap * 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = candidate;
        }
    }
}

/* Return the nth prime (1-indexed) */
static seq_t nth_prime(idx_t n)
{
    if (n == 0) return 0;
    ensure_primes(n);
    return (seq_t)primes[n - 1];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t p = nth_prime(n);
    seq_t r = p;
    /* Compute p^6 */
    r *= p; /* p^2 */
    r *= p; /* p^3 */
    r *= p; /* p^4 */
    r *= p; /* p^5 */
    r *= p; /* p^6 */
    return r;
}
