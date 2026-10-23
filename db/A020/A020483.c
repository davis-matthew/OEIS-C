/*
 * A020483  Least prime p such that p+2n is also prime.
 * https://oeis.org/A020483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that p + 2n is also prime (OEIS
 *            A020483)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Return the nth term of the sequence */
seq_t a(idx_t n)
{
    /* a(0) = 2 by definition */
    if (n == 0)
        return (seq_t)2;

    /* 2n is the offset added to the candidate prime */
    idx_t t = n * 2ULL;

    /* Check the first prime (2) explicitly */
    if (is_prime((idx_t)(2 + t)))
        return (seq_t)2;

    /* Candidate prime p starts at 3 and increments by 2 (odd numbers only) */
    idx_t p = 3;
    for (;; p += 2) {
        if (is_prime((idx_t)p) && is_prime((idx_t)(p + t)))
            return (seq_t)p;   /* Found the least prime p with p+2n prime */
    }
}
