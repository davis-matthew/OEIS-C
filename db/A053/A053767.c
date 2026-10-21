/*
 * A053767  Sum of first n composite numbers.
 * https://oeis.org/A053767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 319, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of the first n composite numbers computed via an incremental prime sieve.
   a(0) = 0, a(1) = 4, a(2) = 10, ... */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static state for incremental computation */
    static seq_t *sums   = NULL;   /* sums[k] = a(k), k = 0 .. sums_len-1 */
    static size_t sums_len = 0;    /* number of stored sums */
    static size_t sums_cap = 0;    /* allocated capacity for sums */

    static uint64_t *primes   = NULL;   /* list of known primes */
    static size_t primes_len   = 0;     /* number of primes stored */
    static size_t primes_cap   = 0;     /* allocated capacity for primes */

    static uint64_t current = 4;       /* the next integer to test */

    /* One‑time initialisation */
    if (!sums) {
        sums_cap = 16;
        sums = (seq_t *)malloc(sums_cap * sizeof(seq_t));
        sums[0] = (seq_t)0;          /* a(0) = 0 */
        sums_len = 1;

        primes_cap = 8;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;               /* first prime */
        primes[1] = 3;               /* second prime, required for correct divisibility tests */
        primes_len = 2;
    }

    /* Extend the table until we have an entry for a(n) */
    while ((size_t)n >= sums_len) {
        /* Test whether 'current' is prime */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > current) break;
            if (current % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* New prime – add to the list for future tests */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap << 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = current;
        } else {
            /* Composite – add to the cumulative sum */
            seq_t last_sum = sums[sums_len - 1];
            seq_t new_sum  = last_sum + (seq_t)current;

            if (sums_len == sums_cap) {
                size_t new_cap = sums_cap << 1;
                seq_t *tmp = (seq_t *)realloc(sums, new_cap * sizeof(seq_t));
                sums = tmp;
                sums_cap = new_cap;
            }
            sums[sums_len++] = new_sum;
        }

        ++current;
    }

    return sums[(size_t)n];
}
