/*
 * A086116  Numerator of mean deviation of a symmetrical binomial distribution
 *          on n elements.
 * https://oeis.org/A086116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( n * binomial(n-1, floor(n/2)) / 2^(n-1) )
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the simplified reduction exploiting only powers of two. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Legendre's formula: exponent of 2 in n! */
static idx_t exp_fact2(idx_t n) {
    idx_t exp = 0;
    while (n > 0) {
        n >>= 1;
        exp += n;
    }
    return exp;
}

seq_t a(idx_t n) {
    /* n >= 1 according to the sequence offset */
    if (n == 0) return 0;    /* safe guard, though offset starts at 1 */

    /* Argument for the binomial coefficient */
    idx_t k = n >> 1;           /* floor(n/2) */
    seq_t B = binomial(n - 1, k);

    /* exponent of 2 in n */
    idx_t v_n = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++v_n;
    }

    /* exponent of 2 in the binomial coefficient using Legendre */
    idx_t vB = exp_fact2(n - 1) - exp_fact2(k) - exp_fact2((n - 1) - k);

    /* total number of twos that can be cancelled */
    idx_t shift = v_n + vB;
    if (shift > n - 1) shift = n - 1;

    /* distribute the shift between n and B */
    idx_t shiftN = (shift <= v_n) ? shift : v_n;
    idx_t shiftB = shift - shiftN;

    /* compute the reduced numerator */
    seq_t res = ((seq_t)(n >> shiftN)) * (B >> shiftB);
    return res;
}
