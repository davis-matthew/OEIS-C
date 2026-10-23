/*
 * A031374  a(n) = prime(8*n - 1).
 * https://oeis.org/A031374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 817, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, uint64_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the prime number at index 8*n-1 (1‑based) */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* utilities for prime generation */
#include "../utils/build_primes_upto.h"
#include "../utils/uint64_to_seq.h"

/* a(n) = 8n‑th prime offset by –1 */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined for n = 0, return 0 as a safe sentinel */
        return (seq_t)0;
    }

    /* target index of the prime (1‑based) */
    idx_t target = 8 * n - 1;

    /* static cache of primes */
    static seq_t *primes = NULL;          /* array of seq_t primes */
    static size_t primes_cnt = 0;         /* number of primes cached */

    /* enlarge cache until it contains the target prime */
    if (primes_cnt < (size_t)target) {

        /* initial value bound for the target prime:
           p_n < n*(log n + log log n) for n >= 6, use a safe multiplier */
        idx_t limit = target * 15 + 100;
        if (limit < 100) limit = 100;

        while (primes_cnt < (size_t)target) {

            /* build primes up to 'limit' (value) */
            uint64_t *raw_primes = NULL;
            size_t raw_cnt = 0;
            build_primes_upto(limit, &raw_primes, &raw_cnt);

            /* convert to seq_t and store in new cache */
            seq_t *new_primes = (seq_t *)malloc(raw_cnt * sizeof(seq_t));
            for (size_t i = 0; i < raw_cnt; ++i) {
                new_primes[i] = uint64_to_seq(raw_primes[i]);
            }

            /* replace old cache, free raw array */
            free(raw_primes);
            free(primes);
            primes = new_primes;
            primes_cnt = raw_cnt;

            /* increase bound for next iteration if needed */
            limit = limit * 2 + 10;
        }
    }

    /* prime indices are 1‑based in the cache */
    return primes[(size_t)(target - 1)];
}
