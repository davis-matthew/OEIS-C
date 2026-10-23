/*
 * A129488  Smallest odd prime dividing binomial(2n,n).
 * https://oeis.org/A129488
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest odd prime dividing binomial(2n,n)  (offset 2) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Exponent of prime p in n! (Legendre's formula) */
static uint64_t exp_in_factorial(idx_t n, uint64_t p)
{
    uint64_t exp = 0;
    uint64_t m = (uint64_t)n;
    while (m) {
        m /= p;
        exp += m;
    }
    return exp;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2; behaviour below the offset is
       irrelevant, but we guard it here. */
    if (n < 2) return (seq_t)0;

    idx_t two_n = n << 1;                 /* 2*n */
    /* Test odd primes starting with 3 until one divides the binomial
       coefficient. The known maximum value for g(n) up to 10 000 is
       13, so this loop terminates very quickly. */
    for (uint64_t p = 3; ; p += 2) {
        if (!is_prime((idx_t)p)) continue;
        uint64_t exp = exp_in_factorial(two_n, p)
                     - 2 * exp_in_factorial(n, p);
        if (exp > 0) return (seq_t)p;   /* first odd prime with positive exponent */
    }
}
