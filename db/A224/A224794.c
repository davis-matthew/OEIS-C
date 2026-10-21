/*
 * A224794  Smallest prime p such that 2n+1 = 2p - q^3 for some odd prime q,
 *          or 0 if no such prime exists.
 * https://oeis.org/A224794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that 2n+1 = 2p – q³ * for an odd
 *            prime q, or 0 if no such prime exists. * * The implementation
 *            searches odd primes q in increasing order. * For each q it
 *            computes p = n + (q³+1)/2 and tests whether p is prime. * If no
 *            solution is found after a large number of trials (...
 * Range:     u8 none, u16 n <= 5, u32 n <= 5, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   floor_cube_root, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Utility prototypes provided by the build system */
#include "../utils/floor_cube_root.h"    /* seq_t floor_cube_root(seq_t x); */
#include "../utils/is_prime_big.h"       /* bool is_prime_big(seq_t n); */
#include "../utils/next_prime.h"         /* uint64_t next_prime(uint64_t start); */

/* Return a(n) */
seq_t a(idx_t n)
{
    /* 2n+1 as seq_t */
    seq_t two_n_plus1 = ((seq_t)n) * (seq_t)2 + (seq_t)1;

    /* If 2n+1 is an odd cube, result is 0. */
    seq_t m = floor_cube_root(two_n_plus1);
    seq_t m3 = m * m * m;
    if (m3 == two_n_plus1)
        return (seq_t)0;

    /* Search odd primes q in increasing order. */
    uint64_t q_start = 3;          /* smallest odd prime */
    size_t  iterations = 0;        /* safety counter */

    while (1) {
        /* Convert current q to seq_t */
        seq_t q_seq = (seq_t)q_start;

        /* Compute q³ and the candidate p */
        seq_t q3   = q_seq * q_seq * q_seq;
        seq_t half = (q3 + (seq_t)1) / (seq_t)2;
        seq_t p    = (seq_t)n + half;

        if (is_prime_big(p))
            return p;              /* first prime p found */

        /* Advance to next odd prime */
        q_start = next_prime(q_start + 1);
        if (q_start == 0)          /* overflow or error */
            break;

        iterations++;
        if (iterations > 100000)   /* safety break */
            break;
    }

    /* If we exit the loop, no suitable prime was found. */
    return (seq_t)0;
}
