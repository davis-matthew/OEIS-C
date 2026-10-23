/*
 * A181669  Primes p of the form 6n-1 such that p-1 is a semiprime and p+2 is
 *          prime or prime squared.
 * https://oeis.org/A181669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th prime p of the form 6k‑1 such that
   p−1 is a semiprime and p+2 is either prime or a perfect square
   of a prime.  The sequence is A181669.  The function computes each
   term by a simple search; no dynamic allocation is required. */

#include <stdbool.h>
#include <stdint.h>
/* UTILITIES ------------------------------------------------------------ */
#include "../utils/is_prime_big.h"        /* bool is_prime_big(seq_t) */
#include "../utils/smallest_prime_factor.h"   /* uint64_t smallest_prime_factor(uint64_t) */
#include "../utils/floor_sqrt.h"          /* seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* The sequence is offset 1, so a(1) is the first term. */
    if (n == 0) return (seq_t)0; /* Undefined; return 0 for safety */

    idx_t seen = 0;               /* how many terms we have found */
    uint64_t candidate = 5;       /* first prime of the form 6k‑1 */

    for (;; candidate += 6) {
        if (!is_prime_big((seq_t)candidate))
            continue;                                /* not prime */

        uint64_t t = candidate - 1;
        uint64_t spf = smallest_prime_factor(t);
        if (spf == t)                    /* t is prime → not semiprime */
            continue;
        uint64_t other = t / spf;
        if (!is_prime_big((seq_t)other) /* other not prime → not semiprime */
            )
            continue;

        uint64_t x = candidate + 2;
        bool ok = false;
        if (is_prime_big((seq_t)x))      /* p+2 is prime */
            ok = true;
        else {
            seq_t sq = floor_sqrt((seq_t)x);
            uint64_t root = (uint64_t)sq;
            if (root * root == x && is_prime_big((seq_t)root))
                ok = true;                 /* p+2 is a prime square */
        }
        if (!ok)
            continue;

        /* a candidate satisfying all conditions */
        seen++;
        if (seen == n)
            return (seq_t)candidate;
    }
}
