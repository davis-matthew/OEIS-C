/*
 * A229297  Number of solutions to x^2 == n (mod 2*n) for 0 <= x < 2*n.
 * https://oeis.org/A229297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of solutions to x^2 ≡ n (mod 2*n) for 0 ≤ x < 2*n
 *            Multiplicative: a(2^e) = 0 if e is odd, a(2^e) = 2^(floor(e/2))
 *            if e is even; a(p^e) = p^(floor(e/2)) for odd primes p
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp for seq_t using exponentiation by squaring.
   seq_t may be an arbitrary‑precision integer type. */
static seq_t power_seq(seq_t base, uint64_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1U) result *= base;
        exp >>= 1U;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* Undefined, but keep behaviour safe */
    seq_t res = (seq_t)1;
    uint64_t tmp = (uint64_t)n;

    /* Handle factor 2 separately */
    uint64_t e2 = 0;
    while ((tmp & 1ULL) == 0ULL) {
        ++e2;
        tmp >>= 1;
    }
    if (e2 & 1ULL) /* odd exponent → result is zero */
        return (seq_t)0;
    if (e2) {
        /* res *= 2^(e2/2) */
        res *= (seq_t)(1ULL << (e2 >> 1));   /* 2^(floor(e2/2)) */
    }

    /* For odd primes */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0ULL) {
            uint64_t exp = 0;
            while (tmp % p == 0ULL) {
                ++exp;
                tmp /= p;
            }
            uint64_t half = exp >> 1;          /* floor(exp/2) */
            if (half) {
                res *= power_seq((seq_t)p, half);
            } else {
                /* exp == 1 -> half == 0, no multiplication needed */
            }
        }
    }

    /* Remaining prime factor > 1 */
    if (tmp > 1ULL) {
        /* exponent is 1, so floor(1/2)=0 -> multiply by 1 */
        /* No change to res */
    }

    return res;
}
