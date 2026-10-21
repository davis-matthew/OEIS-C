/*
 * A060420  Least decimal digit in n-th prime.
 * https://oeis.org/A060420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = minimum decimal digit of the n-th prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
seq_t a(idx_t n)
{
    /* static storage for found primes */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t primes_cap = 0;

    if (n == 0) return 0;          /* undefined, but guard against 0 */

    if (primes_cnt < (size_t)n) {
        /* allocate first chunk */
        if (primes_cap == 0) {
            primes_cap = 32;
            primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
            if (!primes) return 0;        /* allocation failure guard */
            primes[0] = 2;
            primes_cnt = 1;
        }

        uint64_t candidate = primes[primes_cnt - 1] + 1;
        if (candidate % 2 == 0) candidate++;

        /* generate primes until we have at least n */
        while (primes_cnt < (size_t)n) {
            /* integer square root of candidate (small, so simple loop) */
            uint64_t limit = 0;
            {
                uint64_t k = 2;
                while (k * k <= candidate) ++k;
                limit = k - 1;
            }

            bool is_prime = true;
            for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
                if (candidate % primes[i] == 0) {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime) {
                if (primes_cnt >= primes_cap) {
                    primes_cap *= 2;
                    uint64_t *tmp = (uint64_t *)realloc(primes,
                                                        primes_cap * sizeof(uint64_t));
                    if (!tmp) return 0;       /* allocation failure guard */
                    primes = tmp;
                }
                primes[primes_cnt++] = candidate;
            }
            candidate += 2;   /* only check odd numbers thereafter */
        }
    }

    /* get the n-th prime (1‑based index) */
    uint64_t prime = primes[n - 1];

    /* find its smallest decimal digit */
    seq_t min_digit = 9;
    while (prime) {
        seq_t d = prime % 10;
        if (d < min_digit) min_digit = d;
        if (min_digit == 0) break;
        prime /= 10;
    }
    return min_digit;
}
