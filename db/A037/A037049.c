/*
 * A037049  Smallest number with two distinct prime factors both of length n.
 * https://oeis.org/A037049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A037049: smallest number with two distinct prime factors both having n decimal digits. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  Find the first two n‑digit primes and return their product. */
    seq_t lower = pow10(n - 1);        /* smallest n‑digit number (10^(n-1)) */

    if (lower < (seq_t)2)              /* safety for n=1 */
        lower = (seq_t)2;

    seq_t p1 = lower;
    while (!is_prime_big(p1))          /* first n‑digit prime */
        ++p1;

    seq_t p2 = p1 + (seq_t)1;
    while (!is_prime_big(p2))          /* next distinct n‑digit prime */
        ++p2;

    return p1 * p2;                    /* product fits exactly in seq_t when possible */
}
