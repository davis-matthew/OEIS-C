/*
 * A135767  sigma_0(n)-omega(n)-Omega(n) (sigma_0 = A000005 = # divisors,
 *          omega = A001221 = # prime factors, Omega = A001222 = # prime
 *          factors with multiplicity).
 * https://oeis.org/A135767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma_0(n) - omega(n) - Omega(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma_0: number of positive divisors
   omega:   number of distinct prime factors
   Omega:   total number of prime factors (with multiplicity) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square‑root: largest r with r*r <= n, safely for 64‑bit n */
static uint64_t isqrt_uint64(uint64_t n) {
    uint64_t low = 0, high = n;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid > n / mid)        /* mid*mid would overflow or be > n */
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n) {
    /* a(1) = 1 */
    if (n == (idx_t)1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;         /* for factorisation work */
    uint64_t temp = nn;

    /* divisor count, omega (distinct), Omega (total) */
    uint64_t divcnt = 1;
    uint64_t omega  = 0;
    uint64_t omage  = 0;  /* Omega */

    uint64_t limit = isqrt_uint64(temp);
    for (uint64_t p = 2; p <= limit && temp > 1; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            divcnt *= (exp + 1);
            omega  += 1;
            omage  += exp;
            limit   = isqrt_uint64(temp);   /* may shrink after division */
        }
    }

    /* Remaining prime factor > 1 */
    if (temp > 1) {
        divcnt *= 2;          /* exponent 1 -> (1+1) */
        omega  += 1;
        omage  += 1;
    }

    /* Compute the required value */
    return (seq_t)(divcnt - omega - omage);
}
