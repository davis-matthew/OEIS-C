/*
 * A135291  Product of the nonzero exponents in the prime factorization of n!.
 * https://oeis.org/A135291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 32, u32 n <= 75, u64 n <= 179,
 *            u128 n <= 436, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A135291: a(n) = product of the non‑zero exponents in the prime
   factorization of n! (a(0)=1, a(1)=1).  Computed directly from the
   prime‑exponent formula in O(π(n)·log n). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */

seq_t a(idx_t n)
{
    /* Base cases: a(0)=a(1)=1. */
    if (n <= 1)
        return 1;

    /* Obtain all primes ≤ n. */
    uint64_t *primes = NULL;
    size_t   prime_cnt = 0;
    build_primes_upto((idx_t)n, &primes, &prime_cnt);

    seq_t prod = 1;

    /* For each prime p ≤ n, compute its exponent in n! and multiply. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t e = 0;
        idx_t t = n;
        while (t) {
            t /= p;
            e += (seq_t)t;          /* t is small, cast to seq_t */
        }
        prod *= e;                   /* e is non‑zero for p ≤ n */
    }

    free(primes);
    return prod;
}
