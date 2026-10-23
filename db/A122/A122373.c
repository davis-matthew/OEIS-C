/*
 * A122373  Expansion of (c(q)^3 + c(q^2)^3) / 27 in powers of q where c() is
 *          a cubic AGM theta function.
 * https://oeis.org/A122373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a is multiplicative. a(2^e) = 4^e, a(3^e) = 9^e, a(p^e) =
 *            (p^(2e+2) - f^(e+1)) / (p^2 - f) for odd prime p>3 where f = 1
 *            if p ≡ 1 (mod 6), f = -1 if p ≡ 5 (mod 6)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 613, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard */

    uint64_t t = (uint64_t)n;
    seq_t result = 1;

    /* Factor n by trial division */
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }

            seq_t val;
            if (p == 2) {
                val = pow_seq( (seq_t)4, (idx_t)e );
            } else if (p == 3) {
                val = pow_seq( (seq_t)9, (idx_t)e );
            } else {
                /* p > 3 */
                seq_t p2   = pow_seq( (seq_t)p, 2 );          /* p^2          */
                seq_t pPow = pow_seq( (seq_t)p, 2*e + 2 );   /* p^(2e+2)     */

                bool p_is_1mod6 = (p % 6U) == 1U; /* primes >3 are 1 or 5 mod 6 */

                seq_t numerator, denominator;

                if (p_is_1mod6) {          /* f = +1 */
                    numerator = pPow - 1;
                    denominator = p2 - 1;
                } else {                    /* f = -1, p % 6 == 5 */
                    /* f^(e+1) = 1 if e+1 even, -1 otherwise */
                    bool even = ((e + 1u) & 1u) == 0u;
                    if (even) {            /* (-1)^(even) = +1 */
                        numerator = pPow - 1;
                    } else {                /* (-1)^(odd)  = -1 */
                        numerator = pPow + 1;     /* subtract -1 */
                    }
                    denominator = p2 + 1;   /* p^2 - (-1) = p^2 + 1 */
                }
                val = numerator / denominator;   /* exact division */
            }
            result = result * val;
        }
    }

    /* If remainder t > 1, it is a prime factor with e=1 */
    if (t > 1) {
        uint64_t p = t;
        seq_t val;
        if (p == 2) {
            val = 4;
        } else if (p == 3) {
            val = 9;
        } else {
            seq_t p2   = pow_seq( (seq_t)p, 2 );
            seq_t pPow = pow_seq( (seq_t)p, 4 );  /* e=1: 2e+2 = 4 */

            bool p_is_1mod6 = (p % 6U) == 1U;
            seq_t numerator, denominator;

            if (p_is_1mod6) {
                numerator = pPow - 1;
                denominator = p2 - 1;
            } else {
                /* f = -1, p%6==5 */
                bool even = ((1 + 1u) & 1u) == 0u;   /* e+1 = 2 even */
                if (even) {
                    numerator = pPow - 1;
                } else {
                    numerator = pPow + 1;
                }
                denominator = p2 + 1;
            }
            val = numerator / denominator;
        }
        result = result * val;
    }

    return result;
}
