/*
 * A240672  Number of the first evil exponents (A001969) in the prime power
 *          factorization of (2n)!.
 * https://oeis.org/A240672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   seq_t a(idx_t n) counts how many of the leading * exponents in
 *            the prime power factorisation of (2n)! * are evil numbers (even
 *            number of 1's in binary). * It uses Legendre’s formula for each
 *            prime p ≤ 2n, * stops counting at the first non‑evil exponent
 * Range:     u8 n <= 319, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Count leading evil exponents in (2n)! */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset is 1 */

    idx_t m = 2 * n;                   /* (2n)! */

    /* Create an array of all primes up to 2n */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(m, &primes, &prime_cnt);

    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];

        /* Exponent of p in (2n)! via Legendre’s formula */
        seq_t exp = 0;
        idx_t t = m;
        while (t >= p) {
            t /= p;
            exp += (seq_t)t;
        }

        /* Test whether exp is evil: even popcount */
        bool even = true;
        seq_t tmp = exp;
        while (tmp != 0) {
            if ((tmp & (seq_t)1) != 0) even = !even;
            tmp >>= 1;
        }
        if (!even) break;              /* first non‑evil, stop counting */

        ++count;
    }

    free(primes);
    return count;
}
