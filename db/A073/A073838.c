/*
 * A073838  Product of primes p satisfying n <= p <= 2n.
 * https://oeis.org/A073838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{p prime, n <= p <= 2*n} p
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 26, u64 n <= 50, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* a(n) computes the product of all primes p with n <= p <= 2*n
 *  idx_t  - unsigned integer type for indices (uint64_t)
 *  seq_t  - the sequence value type (uint64_t or arbitrary‑precision)
 */
seq_t a(idx_t n) {
    /* static cache of primes up to the largest 2*n requested so far */
    static idx_t max_checked = 0;          /* largest number that has been
                                              checked for primality */
    static uint32_t *primes = NULL;        /* sorted array of primes */
    static size_t primes_cnt = 0;          /* number of primes currently in array */
    static size_t primes_cap = 0;          /* current capacity of primes array */

    /* always work with 64‑bit values for intermediate calculations */
    idx_t limit = n * 2;

    /* extend the prime table up to 'limit' if needed */
    if (limit > max_checked) {
        for (idx_t cand = max_checked + 1; cand <= limit; ++cand) {
            int is_prime = 1;
            if (cand < 2) {
                is_prime = 0;
            } else {
                /* test divisibility by known primes up to sqrt(cand) */
                for (size_t i = 0; i < primes_cnt; ++i) {
                    uint32_t p = primes[i];
                    if ((seq_t)p * (seq_t)p > cand) break;
                    if (cand % p == 0) {
                        is_prime = 0;
                        break;
                    }
                }
            }
            if (is_prime) {
                /* grow array if necessary */
                if (primes_cnt + 1 > primes_cap) {
                    size_t new_cap = primes_cap == 0 ? 64 : primes_cap * 2;
                    uint32_t *tmp = (uint32_t *)realloc(primes,
                                                        new_cap * sizeof(uint32_t));
                    if (!tmp) return 0;           /* allocation failed */
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_cnt++] = (uint32_t)cand;
            }
        }
        max_checked = limit;
    }

    /* multiply primes in the interval [n, 2*n] */
    seq_t prod = 1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if (p < n) continue;
        if (p > limit) break;
        prod *= (seq_t)p;
    }
    return prod;
}
