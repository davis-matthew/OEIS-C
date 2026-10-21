/*
 * A091368  Primes p such that the sum of the digits of p is not prime, but
 *          the sum of each digit raised to the 4th power is prime.
 * https://oeis.org/A091368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 69, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A091368 – primes p such that the sum of the digits of p is
   not prime, but the sum of each digit raised to the 4th power is prime.
   The function a(idx_t n) returns the n-th term (1‑based).  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"        /* bool is_prime(idx_t x); */
#include "../utils/is_prime_small.h" /* bool is_prime_small(uint64_t num); */

/* 4th powers of digits 0–9.  Values fit in 16 bits. */
static const uint32_t POW4[10] = {
    0,      /* 0^4 */
    1,      /* 1^4 */
    16,     /* 2^4 */
    81,     /* 3^4 */
    256,    /* 4^4 */
    625,    /* 5^4 */
    1296,   /* 6^4 */
    2401,   /* 7^4 */
    4096,   /* 8^4 */
    6561    /* 9^4 */
};

/* Return true if p satisfies the OEIS condition. */
static bool valid_prime(uint64_t p)
{
    unsigned digit_sum = 0;
    unsigned pow4_sum  = 0;

    while (p > 0) {
        unsigned d = (unsigned)(p % 10);
        digit_sum += d;
        pow4_sum  += POW4[d];
        p /= 10;
    }
    return (!is_prime_small(digit_sum)) && is_prime_small(pow4_sum);
}

/* Compute the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for n = 0 – return 0 as placeholder */

    idx_t count = 0;
    uint64_t cur = 2;        /* start from the first prime */

    for (;; ++cur) {
        if (is_prime(cur) && valid_prime(cur)) {
            ++count;
            if (count == n) {
                return (seq_t)cur;
            }
        }

        /* Safety: stop if the 64‑bit counter would overflow. */
        if (cur == UINT64_MAX) break;
    }

    /* n is too large for this implementation (overflowed). */
    return 0;
}
