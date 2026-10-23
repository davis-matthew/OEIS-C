/*
 * A048795  Concatenate p p times, where p runs through the primes.
 * https://oeis.org/A048795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of the nth prime p, repeated p times. *
 *            That is * a(n) = p × (10^(len(p) · p) – 1) / (10^len(p) – 1) *
 *            where len(p) is the number of decimal digits of p
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, digits, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* n ≥ 1 as per OEIS offset */
    uint32_t p = get_prime(n);          /* nth prime, fits in 32‑bit */
    idx_t lenp = digits(p);             /* number of decimal digits of p */

    /* Compute the denominator 10^lenp – 1  */
    seq_t ten_len = pow10(lenp);        /* 10^lenp  */
    seq_t denom   = ten_len - 1;        /* 10^lenp – 1 */

    /* Compute the numerator 10^(lenp·p) – 1  */
    seq_t exp     = (seq_t)lenp * (seq_t)p;      /* exponent = lenp * p */
    seq_t numer   = big_pow((seq_t)10, exp) - 1; /* 10^exp – 1 */

    /* Assemble the final value */
    seq_t q = numer / denom;            /* division is exact */
    return q * (seq_t)p;                /* multiply by the prime p */
}
