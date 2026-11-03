/*
 * A133790  A014963*A100994.
 * https://oeis.org/A133790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * p if n is a (non‑trivial) power of a prime p,
 *            otherwise a(n) = 1. (a(1) = 1). This is equivalent to the
 *            product of A014963(n) and A100994(n)
 * Range:     u8 n <= 16, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if n is a power of a prime, setting *prime to that prime.
   For n == 1 or composite numbers with more than one distinct prime
   factor, returns false.  */
static bool is_prime_power_and_factor(idx_t n, idx_t *prime)
{
    if (n <= 1)
        return false;                  /* 1 is handled separately */

    idx_t temp = n;
    idx_t p = 0;

    for (idx_t i = 2; i <= temp / i; ++i) {
        if (temp % i == 0) {
            p = i;
            break;
        }
    }

    if (p == 0) {                /* n is prime */
        *prime = temp;
        return true;
    }

    /* Remove all factors of p */
    while (temp % p == 0)
        temp /= p;

    if (temp != 1)              /* another prime factor remains */
        return false;

    *prime = p;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t p;
    if (!is_prime_power_and_factor(n, &p))
        return (seq_t)1;

    /* n and p are each at most 64‑bit; cast to seq_t for multiplication. */
    return (seq_t)n * (seq_t)p;
}
