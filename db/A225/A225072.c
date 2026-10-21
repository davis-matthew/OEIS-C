/*
 * A225072  Number of first-quadrant Gaussian primes at taxicab distance 2n-1
 *          from the origin.
 * https://oeis.org/A225072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 550, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: For each n, count the number of Gaussian primes
 * a+bi with a,b ≥ 0, a+b = 2n‑1. An axis prime (a=0 or b=0) is
 * counted once if the rational prime is ≡3 (mod 4).
 * A Gaussian prime in the first quadrant is (a,b) with a>0, b>0,
 * and a²+b² being a rational prime.
 *
 * This routine enumerates all pairs (a,b) with a+b=2n‑1 and uses
 * the provided `is_prime` helper to test primality.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t d = 2 * n - 1;          /* taxicab distance */

    /* Count an axis prime once if d is a rational prime ≡3 (mod 4). */
    if (is_prime(d) && (d & 3) == 3)
        count++;

    /* Count primes with both coordinates >0. */
    for (idx_t a1 = 1; a1 < d; ++a1)
    {
        idx_t b1 = d - a1;
        seq_t s = (seq_t)a1 * a1 + (seq_t)b1 * b1;
        if (is_prime((idx_t)s))
            count++;
    }
    return count;
}
