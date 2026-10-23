/*
 * A101781  Lower bound b of twin primes pairs such that b's digital reverse
 *          is also prime.
 * https://oeis.org/A101781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_uint64, reverse_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101781: lower bound of twin prime pair (b, b+2) such that 
   reverse(b) is also prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/reverse_uint64.h"

/* store small primes up to LIMIT for sieving */
static uint64_t *BASE_PRIMES   = NULL;
static size_t   BASE_PRIME_CNT = 0;

/* initialize the base primes once */
static void init_base_primes(void)
{
    if (BASE_PRIME_CNT > 0)
        return;

    const uint64_t LIMIT = 1000000ULL;            /* 1e6 */
    bool *sieve = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!sieve) exit(EXIT_FAILURE);

    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = true;
        }
    }

    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            BASE_PRIME_CNT++;
            BASE_PRIMES = (uint64_t *)realloc(BASE_PRIMES,
                                               BASE_PRIME_CNT * sizeof(uint64_t));
            if (!BASE_PRIMES) exit(EXIT_FAILURE);
            BASE_PRIMES[BASE_PRIME_CNT - 1] = i;
        }
    }

    free(sieve);
}

/* compute the nth term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined; return 0 for safety */
        return 0;

    init_base_primes();

    idx_t  count        = 0;
    uint64_t prev_prime = 0;

    const uint64_t SEGMENT_SIZE = ((uint64_t)1 << 20);  /* 1,048,576 */
    uint64_t low = 2;

    while (true) {
        uint64_t high   = low + SEGMENT_SIZE - 1;
        size_t  len     = (size_t)(high - low + 1);

        bool *segment = (bool *)calloc(len, sizeof(bool));
        if (!segment) exit(EXIT_FAILURE);

        /* mark composites in the current segment */
        for (size_t i = 0; i < BASE_PRIME_CNT; ++i) {
            uint64_t p  = BASE_PRIMES[i];
            uint64_t p2 = p * p;
            if (p2 > high)
                break;
            uint64_t start = (low + p - 1) / p * p;
            if (start < p * 2)
                start = p * 2;
            for (uint64_t j = start; j <= high; j += p)
                segment[j - low] = true;
        }
        if (low == 1)
            segment[0] = true;   /* 1 is not prime */

        /* iterate through primes in the segment */
        for (size_t idx = 0; idx < len; ++idx) {
            uint64_t num = low + idx;
            if (num < 2 || segment[idx])
                continue;        /* composite or <2 */

            /* num is prime */
            if (prev_prime > 0 && num - prev_prime == 2) {
                uint64_t rev = reverse_uint64(prev_prime);
                if (is_prime_uint64(rev)) {
                    count++;
                    if ((idx_t)count == n) {
                        free(segment);
                        return (seq_t)prev_prime;
                    }
                }
            }
            prev_prime = num;
        }

        free(segment);
        low = high + 1;
    }
}
