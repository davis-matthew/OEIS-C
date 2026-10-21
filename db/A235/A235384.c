/*
 * A235384  Number of involutions in the group Aff(Z/nZ).
 * https://oeis.org/A235384
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, look, mult, nice
 * Formula:   a(n) = if n = 2^m * Π p_i^r_i (p_i>2) then a(n) = coeff(m) * Π
 *            (p_i^r_i + 1) where coeff(0)=1, coeff(1)=2, coeff(2)=6,
 *            coeff(m)=4 + 2^(m-1) + 2^m for m≥3
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp using only seq_t multiplication */
static seq_t pow_small(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)base;
    return result;
}

seq_t a(idx_t n)
{
    /* n >= 2 according to the offset */
    idx_t temp = n;
    idx_t m = 0;

    /* count powers of 2 */
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        m++;
    }

    seq_t prod = 1;          /* product over odd primes: Π (p^r + 1) */

    idx_t rest = temp;       /* remaining odd part to factor */

    while (rest > 1) {
        idx_t p = 0;
        /* trial division for odd primes, up to sqrt(rest) */
        for (idx_t d = 3; d * d <= rest; d += 2) {
            if (rest % d == 0) { p = d; break; }
        }
        if (p == 0) p = rest;   /* rest itself is prime */

        idx_t exp = 0;
        while (rest % p == 0) { rest /= p; exp++; }

        seq_t part = pow_small(p, exp);
        part = part + 1;   /* (p^exp + 1) */
        prod = prod * part;
    }

    /* coefficient depending on m */
    seq_t coeff;
    switch (m) {
        case 0: coeff = 1; break;
        case 1: coeff = 2; break;
        case 2: coeff = 6; break;
        default:
            coeff = 4 + pow_small(2, m - 1) + pow_small(2, m);
            break;
    }

    return coeff * prod;
}
