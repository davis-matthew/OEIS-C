/*
 * A124611  a(n) = sum of the positive integers k, k<=n, where each positive
 *          integer <=k and coprime to k is also coprime to n.
 * https://oeis.org/A124611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} [ for all prime p|n with p<=k, p divides k
 *            ]
 * Range:     u8 n <= 22, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   sumMultiples
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sumMultiples.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for 0, but safe */

    /* factor n into distinct primes */
    uint64_t n_tmp = (uint64_t)n;
    idx_t primes[64];
    idx_t pcnt = 0;

    for (uint64_t p = 2; p * p <= n_tmp; ) {
        if (n_tmp % p == 0) {
            primes[pcnt++] = (idx_t)p;
            while (n_tmp % p == 0) n_tmp /= p;
        }
        p = (p == 2) ? 3 : p + 2;   /* skip even numbers after 2 */
    }
    if (n_tmp > 1) {
        primes[pcnt++] = (idx_t)n_tmp;
    }

    seq_t result = 0;
    idx_t prev = 1;
    idx_t prod = 1;  /* product of primes already processed */

    for (idx_t i = 0; i < pcnt; i++) {
        idx_t p = primes[i];

        idx_t L = prev;
        idx_t R = (p > 1) ? (p - 1) : 0;
        if (R > n) R = n;

        if (L <= R) {
            result += sumMultiples(L, R, prod);
        }

        /* include the new prime factor */
        prod *= p;
        prev = p;
    }

    /* final segment after the last prime */
    idx_t L = prev;
    idx_t R = n;
    if (L <= R) {
        result += sumMultiples(L, R, prod);
    }

    return result;
}
