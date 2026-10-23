/*
 * A173614  a(n) = lcm_{p is prime and divisor of n} p-1.
 * https://oeis.org/A173614
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm_{p prime, p|n} (p-1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* ------------------------------------------------------------ */
/* Euclidean algorithm for unsigned numbers of type seq_t       */
static seq_t
gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------ */
/* lcm(a,b) for unsigned types                                 */
static seq_t
lcm_seq(seq_t a, seq_t b)
{
    if (a == 0 || b == 0)
        return 0;
    seq_t g = gcd_seq(a, b);
    return (a / g) * b;
}

/* ------------------------------------------------------------ */
/* Main function for OEIS sequence A173614                     */
seq_t
a(idx_t n)
{
    /* behaviour below the offset (n < 2) is irrelevant */
    if (n < 2)
        return 0;

    /* n fits in a 64‑bit unsigned integer */
    uint64_t m = (uint64_t)n;

    /* factorise m: primes[] will contain distinct prime divisors
       within the maximum of 64 primes for a 64‑bit number             */
    uint64_t primes[64];
    uint64_t exponents[64];   /* exponents are unused here */
    size_t cnt = 0;

    factor_u64(m, primes, exponents, &cnt);

    /* compute the lcm of (p-1) over the distinct prime divisors */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t val = (seq_t)(primes[i] - 1);
        result = lcm_seq(result, val);
    }

    return result;
}
