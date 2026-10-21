/*
 * A050470  a(n) = Sum_{d|n, n/d == 1 (mod 4)} d^2 - Sum_{d|n, n/d == 3 (mod
 *          4)} d^2.
 * https://oeis.org/A050470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n, n/d == 1 (mod 4)} d^2 - Sum_{d|n, n/d == 3 (mod
 *            4)} d^2 The function uses the multiplicative property of the
 *            convolution of n^2 with the Dirichlet character χ₄. For odd
 *            primes p we have a(p^e) = (p^(2(e+1)) - 1)/(p^2 - 1) if p ≡ 1
 *            (mod 4) (p^(2(e+1)) + (-1)^e)/(p^2 + 1) if p ≡ 3...
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 4131, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Exponentiation by squaring: computes base^exp where exp is idx_t */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;     /* undefined at zero, fallback */

    seq_t result = 1;
    uint64_t tmp = (uint64_t)n;

    /* Handle factor 2 separately */
    int exp2 = 0;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        exp2++;
    }
    if (exp2 > 0) {
        seq_t term = pow_seq((seq_t)2, 2 * exp2);
        result *= term;
    }

    /* Factor remaining odd part using trial division */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }

            seq_t p2 = (seq_t)p * (seq_t)p;          /* p^2           */
            seq_t p_pow = pow_seq(p2, exp + 1);      /* p^(2(e+1)) */

            seq_t term;
            if (p % 4 == 1) {
                /* (p_pow - 1) / (p2 - 1) */
                term = (p_pow - 1) / (p2 - 1);
            } else { /* p % 4 == 3 */
                /* (p_pow + (-1)^exp) / (p2 + 1) */
                seq_t numer = (exp % 2 == 0) ? (p_pow + 1) : (p_pow - 1);
                term = numer / (p2 + 1);
            }

            result *= term;
        }
    }

    /* Any remaining prime factor > sqrt(n) */
    if (tmp > 1) {
        uint64_t p = tmp;
        int exp = 1;  /* exponent must be 1 */

        seq_t p2 = (seq_t)p * (seq_t)p;
        seq_t p_pow = pow_seq(p2, exp + 1);

        seq_t term;
        if (p % 4 == 1) {
            term = (p_pow - 1) / (p2 - 1);
        } else {
            seq_t numer = (exp % 2 == 0) ? (p_pow + 1) : (p_pow - 1);
            term = numer / (p2 + 1);
        }
        result *= term;
    }

    return result;
}
