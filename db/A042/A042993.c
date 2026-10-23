/*
 * A042993  Primes congruent to {0, 2, 3} mod 5.
 * https://oeis.org/A042993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes congruent to 0, 2, or 3 modulo 5 by sieving. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined, but safe return */

    /* Rough upper bound: 30 * n + 100 guarantees enough primes
       for n up to at least 10 000 (and much more). */
    idx_t limit = n * 30 + 100;
    if (limit < 1000) limit = 1000;

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    idx_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 5 || (p % 5 == 2) || (p % 5 == 3)) {
            ++count;
            if (count == n) {
                seq_t res = (seq_t)p;
                free(primes);
                return res;
            }
        }
    }
    free(primes);

    /* In the unlikely event the estimate was insufficient,
       double the limit and repeat until we find the n‑th term. */
    while (true) {
        limit *= 2;
        build_primes_upto(limit, &primes, &prime_cnt);
        count = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p == 5 || (p % 5 == 2) || (p % 5 == 3)) {
                ++count;
                if (count == n) {
                    seq_t res = (seq_t)p;
                    free(primes);
                    return res;
                }
            }
        }
        free(primes);
    }
}
