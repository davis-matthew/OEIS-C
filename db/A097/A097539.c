/*
 * A097539  Subtract 4 from primes == 1 (mod 4).
 * https://oeis.org/A097539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) – 4, where p(n) is the nth prime congruent to 1 (mod
 *            4)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Only indices ≥ 1 are defined (offset 1). */
    if (n == 0)
        return 0;

    /* Store found primes to accelerate the trial division. */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt    = 0;
    static size_t prime_cap    = 0;

    /* Initialise the prime list with the first two primes. */
    if (prime_cnt == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)realloc(NULL, prime_cap * sizeof(uint64_t));
        if (!prime_list) return 0;
        prime_list[prime_cnt++] = 2;
        prime_list[prime_cnt++] = 3;
    }

    idx_t seen = 0;                     /* how many primes ≡1 (mod 4) have been seen */
    uint64_t candidate = 5;              /* first odd candidate that may be prime */

    while (1) {
        /* Simple trial division using the primes already found. */
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p > candidate / p) break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime for future divisions. */
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (!tmp) return 0;
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = candidate;

            if ((candidate & 3ULL) == 1ULL) {   /* prime ≡ 1 (mod 4) */
                ++seen;
                if (seen == n)
                    return (seq_t)(candidate - 4);   /* a(n) = prime – 4 */
            }
        }
        candidate += 2;  /* only odd numbers need to be checked */
    }
}
