/*
 * A190300  Composite numbers that are not Brazilian.
 * https://oeis.org/A190300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = p_n^2 for n >= 6;
   a(1)=4, a(2)=6, a(3)=9, a(4)=25, a(5)=49
   where p_n denotes the n‑th prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special cases for the first five terms. */
    if (n <= 5) {
        switch ((int)n) {
        case 1: return (seq_t)4;   /* 2^2 */
        case 2: return (seq_t)6;   /* 2 * 3 */
        case 3: return (seq_t)9;   /* 3^2 */
        case 4: return (seq_t)25;  /* 5^2 */
        case 5: return (seq_t)49;  /* 7^2 */
        }
    }

    /* Static cache of primes. Indexed from 0. */
    static uint32_t *prime_cache = NULL;
    static size_t  prime_cap   = 0;
    static size_t  prime_cnt   = 0;

    /* Allocate initial space for primes if needed. */
    if (prime_cache == NULL) {
        prime_cap   = 128;
        prime_cache = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    }

    /* Generate primes until we have at least n of them. */
    if (prime_cnt < (size_t)n) {
        uint64_t candidate = (prime_cnt == 0) ? 2 : (uint64_t)prime_cache[prime_cnt - 1] + 1;

        for (uint64_t num = candidate; ; ++num) {
            bool is_prime = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = (uint64_t)prime_cache[i];
                if (p * p > num)
                    break;
                if (num % p == 0) {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime) {
                /* Store the new prime. */
                if (prime_cnt >= prime_cap) {
                    prime_cap *= 2;
                    prime_cache = (uint32_t *)realloc(prime_cache,
                                                      prime_cap * sizeof(uint32_t));
                }
                prime_cache[prime_cnt++] = (uint32_t)num;

                if (prime_cnt == (size_t)n)
                    break;   /* we now have the nth prime */
            }
        }
    }

    /* Compute the square of the nth prime. */
    seq_t p = (seq_t)prime_cache[n - 1];
    return p * p;
}
