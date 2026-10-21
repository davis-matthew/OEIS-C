/*
 * A084061  Square number array read by antidiagonals.
 * https://oeis.org/A084061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is obtained by reading the array * T(i,j) = ((i -
 *            sqrt(j))^i + (i + sqrt(j))^i)/2 * along its antidiagonals
 *            (enumerating the pairs (i,j) with * i ≥ 0, j ≥ 0, in order of
 *            increasing i+j and, for equal * i+j, increasing i). a(0) = 1. *
 *            * The sequence starts: * a(0)=1, a(1)=1, a(2)=1, a(3)=1, a...
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 64, u64 n <= 151,
 *            u128 n <= 404, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* integer square root for a 64‑bit unsigned integer */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t x = n;
    idx_t y = (x + n / x) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* integer power (base^exp) for seq_t with exponent up to 64‑bit */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* special case for the very first position */
    if (n == 0)
        return (seq_t)1;

    /* find the diagonal d such that a(n) lies on it:
     * number of entries up to diagonal d = (d+1)(d+2)/2
     * solve d ≈ floor((sqrt(8n+1)-1)/2)     */
    idx_t m            = 8 * n + 1;
    idx_t sqrt_m       = isqrt_u64(m);
    idx_t d            = (sqrt_m - 1) / 2;

    /* compute the number of entries before this diagonal */
    idx_t prev_total   = d * (d + 1) / 2;

    /* position of (i,j) within the diagonal */
    idx_t offset_in_diag = n - prev_total;
    idx_t i = offset_in_diag;          /* row index    */
    idx_t j = d - offset_in_diag;      /* column index */

    /* compute T(i,j) = sum_{t=0}^{⌊i/2⌋} C(i,2t) * i^(i-2t) * j^t */
    seq_t result = (seq_t)0;
    for (idx_t t = 0; (2 * t) <= i; ++t) {
        seq_t binom_k = binomial(i, 2 * t);
        seq_t pow_i   = pow_seq((seq_t)i, i - 2 * t);
        seq_t pow_j   = pow_seq((seq_t)j, t);
        seq_t term    = binom_k * pow_i;
        term          *= pow_j;
        result        += term;
    }
    return result;
}
