/*
 * A085107  Starting with n-1 go on adding the numbers less than n and coprime
 *          to it in descending order until one gets a prime. a(n) = this
 *          prime, or 0 if no such number exists.
 * https://oeis.org/A085107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of descending integers less than n that are coprime
 *            to n until the cumulative sum is prime. Return that prime; if no
 *            such sum exists, return 0
 * Range:     u8 n <= 28, u16 n <= 565, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_seq_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_seq_prime.h"

/* Euclidean algorithm for unsigned 64‑bit integers */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* a(1) and a(2) are 0 by definition. Avoid underflow when n == 1. */
    if (n < 3) return 0;

    seq_t sum = (seq_t)(n - 1);          /* start with the largest coprime candidate */
    if (is_seq_prime(sum)) return sum;

    /* Descend through the remaining numbers less than n */
    for (idx_t i = n - 2; i > 0; --i) {
        if (gcd_uint64(i, n) == 1) {
            sum += (seq_t)i;
            if (is_seq_prime(sum)) return sum;
        }
    }
    return 0;   /* no prime sum was found */
}
