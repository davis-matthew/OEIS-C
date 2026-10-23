/*
 * A024700  a(n) = (prime(n+2)^2 - 1)/3.
 * https://oeis.org/A024700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n+2)^2 - 1) / 3
 * Range:     u8 n <= 4, u16 n <= 83, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For any other n we return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Fetch the (n+2)-th prime. get_prime() returns a 32‑bit value,
       sufficient for all n that fit in seq_t. */
    uint64_t p64 = get_prime(n + 2);
    seq_t p = (seq_t)p64;

    /* Compute (p*p - 1) / 3 using only seq_t arithmetic. */
    seq_t square = p * p;
    seq_t res = (square - (seq_t)1) / (seq_t)3;
    return res;
}
