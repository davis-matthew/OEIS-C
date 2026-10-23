/*
 * A106963  Primes of the form 4x^2 + 5y^2.
 * https://oeis.org/A106963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 804, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   integer_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A106963
 *
 * The sequence consists of all primes p that can be written as
 *   p = 4*x^2 + 5*y^2   (x,y ≥ 0, not both zero).
 *
 * This implementation scans odd integers starting from 5,
 * checks for primality and tests whether a representation
 * exists by trying all feasible y.  The representation test
 * uses integer arithmetic only and works for any seq_t.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/integer_sqrt.h"

/* Check whether a prime p (p ≥ 5) is representable as 4x^2 + 5y^2.
 * For p=5 the representation is 4*0^2+5*1^2.  For larger primes
 * y must satisfy 5*y^2 ≤ p and (p-5*y^2) be a multiple of 4.
 * We test all such y and then check that the remaining part
 * is a perfect square.
 */
static bool has_representation(uint64_t p)
{
    if (p == 5) return true;
    uint64_t ymax = (uint64_t)integer_sqrt(p / 5);
    for (uint64_t y = 1; y <= ymax; ++y) {
        uint64_t val = 5 * y * y;
        if (val > p) break;
        uint64_t rest = p - val;
        if (rest & 3) continue;           /* rest must be a multiple of 4   */
        uint64_t sq = rest >> 2;          /* sq = x^2                        */
        uint64_t r  = (uint64_t)integer_sqrt(sq);
        if (r * r == sq) return true;     /* perfect square                 */
    }
    return false;
}

/* Compute the n-th term of the sequence, with 1‑based indexing. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* defined only for n≥1; harmless return */
    /* We start at the first known prime 5. */
    idx_t count = 1;
    if (n == 1) return (seq_t)5;

    /* Scan odd integers ≥7.  Every prime ≥7 is odd. */
    for (uint64_t p = 7; ; p += 2) {
        if (!is_prime(p)) continue;
        if (has_representation(p)) {
            ++count;
            if (count == n) return (seq_t)p;
        }
    }
}
