/*
 * A038969  Primes p such that 83 is a square mod p.
 * https://oeis.org/A038969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038969: primes p such that 83 is a quadratic residue modulo p
 * (OEIS offset 1).  The terms are obtained by testing successive
 * primes p in increasing order; a prime p is kept if
 *   (p == 83)  or  83^((p-1)/2) ≡ 1 (mod p)  (Euler’s criterion).
 * The function builds up a cache of found primes so that arbitrary call
 * orders are supported.  It uses the supplied utilities:
 *   - bool is_prime(idx_t x);        (../utils/is_prime.h)
 *   - uint64_t powmod64(uint64_t b, uint64_t e, uint64_t m);
 *     (../utils/powmod64.h)
 * all calculations fit into uint64_t for the intended ranges.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x);          */
#include "../utils/powmod64.h"   /* uint64_t powmod64(u64,u64,u64); */

seq_t a(idx_t n)
{
    /* --- memoization tables --------------------------------------- */
    static seq_t *cache = NULL;
    static size_t  len   = 0;   /* number of stored terms              */
    static size_t  cap   = 0;   /* allocated capacity                  */
    static idx_t  next  = 3;    /* next integer candidate to test      */

    /* --- (1) if we already have n terms, deliver result           */
    if (len >= (size_t)n)
        return cache[(size_t)(n - 1)];

    /* --- (2) create initial cache if empty                         */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* --- (3) extend cache until it contains at least n terms       */
    for (uint64_t cand = next; ; ++cand) {
        /* skip even numbers (except 2, which is not needed) */
        if ((cand & 1) == 0)
            continue;

        if (!is_prime(cand))
            continue;

        /* check the quadratic residue condition */
        int ok;
        if (cand == 83ULL)
            ok = 1;
        else {
            uint64_t r = powmod64(83ULL, (cand - 1) >> 1, cand);
            ok = (r == 1ULL);
        }

        if (ok) {
            /* grow the cache if necessary */
            if (len == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)cand;
            if (len >= (size_t)n) {
                next = cand + 2;   /* next odd number after current cand */
                return cache[(size_t)(n - 1)];
            }
        }
    }
}
