/*
 * A121052  Smallest positive integer m for which n^m is congruent to 1 modulo
 *          n^2+n-1.
 * https://oeis.org/A121052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 256, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 74 terms through n = 9346 (2026-11-01)
 * Helpers:   powmod, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest positive integer m such that
   n^m ≡ 1 (mod n^2 + n – 1).  For n = 1 the
   convention a(1) = 1 is used (see OEIS entry).  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    /* Special case n = 1.  Modulo 1 is trivial, but the OEIS
       defines a(1) = 1. */
    if (n == 1)
        return (seq_t)1;

    /* Convert the index to the seq_t arithmetic type. */
    seq_t ns  = (seq_t)n;

    /* Modulus m = n^2 + n – 1.  It fits into seq_t for all
       n that the test suite uses with the current seq_t size. */
    seq_t mod = ns * ns + ns - (seq_t)1;

    /* Compute φ(mod).  The helper calc_phi does it exactly
       by factoring mod. */
    seq_t phi = totient_seq(mod);

    /* We look for the minimal divisor d of phi such that
       n^d ≡ 1 (mod mod).  First factor phi by trial division.
       As phi is at most ≈ mod, a simple O(√phi) factorisation
       is fast enough for n up to at least 10 000. */

    seq_t order = phi;        /* current candidate for the order   */
    seq_t temp  = phi;        /* value to factorise                */

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {              /* p is a prime factor of phi     */
            /* Remove all powers of p from the factorisation.  */
            while (temp % p == 0)
                temp /= p;

            /* Try to strip this prime factor from the order
               as many times as possible while preserving the
               congruence. */
            while (order % p == 0) {
                seq_t cand = order / p;
                if (powmod(ns, (uint64_t)cand, mod) == (seq_t)1)
                    order = cand;
                else
                    break;
            }
        }
    }

    /* If a prime factor > sqrt(phi) remains, process it. */
    if (temp > 1) {
        uint64_t p = (uint64_t)temp;
        while (order % p == 0) {
            seq_t cand = order / p;
            if (powmod(ns, (uint64_t)cand, mod) == (seq_t)1)
                order = cand;
            else
                break;
        }
    }

    return order;
}
