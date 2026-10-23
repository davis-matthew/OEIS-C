/*
 * A195377  2.5-gap primes: Prime p is a term if there is no prime between
 *          2.5*p and 2.5*q, where q is the next prime after p.
 * https://oeis.org/A195377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A195377 – 2.5‑gap primes a(n) is the n‑th prime p for which no
 *            prime lies strictly between 2.5·p and 2.5·q, where q is the next
 *            prime after p
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    idx_t count = 0;
    uint64_t p = 2;                        /* start with the first prime */

    while (1) {
        uint64_t q = next_prime(p + 1);    /* next prime after p */

        /* lower bound L = floor(5*p/2)   (2.5p with integer floor)   */
        uint64_t L = ((uint64_t)5 * p) >> 1;

        /* upper bound U = ceil(5*q/2). For odd q, (5*q+1)/2 does the ceil. */
        uint64_t U = ((((uint64_t)5 * q) + 1) >> 1);   /*  ceil(5*q/2) */

        /* smallest prime strictly greater than L */
        uint64_t r = next_prime(L);

        /* if there is a prime r with L < r < U, then p is not a term */
        if (r >= U) {
            ++count;
            if (count == n) return (seq_t)p;
        }

        p = q;  /* move to the next prime to examine */
    }
}
