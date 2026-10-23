/*
 * A171014  In the sequence of prime numbers, replace all the '2' digits with
 *          '0' and vice versa.
 * https://oeis.org/A171014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace all digit '2' with '0' and all digit '0' with '2'
 *            in the n-th prime number (with offset 1)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but guard anyway */

    /* static cache of primes */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static size_t prime_cap = 0;
    static uint64_t candidate = 3;   /* will be set to 3 after first prime */

    /* Grow the primes array lazily */
    if (prime_cap == 0) {
        prime_cap = 16;
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    /* Generate primes until we have at least n of them */
    while (prime_cnt < n) {
        if (prime_cnt == 0) {          /* add the first prime 2 */
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
            }
            primes[prime_cnt++] = 2;
            candidate = 3;            /* start testing from 3 and only odds */
            continue;
        }

        if (prime_cnt == prime_cap) {  /* grow if needed */
            prime_cap <<= 1;
            primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
        }

        if (is_prime(candidate))
            primes[prime_cnt++] = candidate;

        candidate += 2;   /* skip even numbers */
    }

    /* p is the n-th prime */
    seq_t p = (seq_t)primes[n - 1];

    /* Transform the prime */
    int len = digit_len(p);
    /* A sufficient stack buffer for the expected size of primes */
    char digits[200];
    seq_t tmp = p;
    for (int i = 0; i < len; ++i) {
        seq_t d = tmp % 10;
        tmp /= 10;
        digits[i] = '0' + (int)d;   /* store least‑significant digit first */
    }

    /* Build the transformed number */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        char c = digits[i];
        char nc = (c == '2') ? '0' : ((c == '0') ? '2' : c);
        result = result * 10 + (seq_t)(nc - '0');
    }

    return result;
}
