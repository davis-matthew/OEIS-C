/*
 * A029834  A discrete version of the Mangoldt function: if n is prime then
 *          floor(log(n)) else 0.
 * https://oeis.org/A029834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log(n)) if n is prime, otherwise 0
 * Range:     u8 n <= 9977, u16 n <= 9977, u32 n <= 9977, u64 n <= 9977,
 *            u128 n <= 9977, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (!is_prime(n))
        return 0;

    /* log(n) is naturally defined for n ≥ 2; n is unsigned and <= 1e4
       for the required test range, so double precision is safe. */
    double ln = log((double)n);
    int val = (int)(ln + 1e-12);   /* epsilon protects against round‑off edge cases */
    return (seq_t)val;
}
