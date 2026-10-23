/*
 * A181278  Number of 2 X n binary matrices M with rows in strictly increasing
 *          order and rows of M*Mtranspose (mod 2) in strictly decreasing
 *          order.
 * https://oeis.org/A181278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n≥2, a(n)=3*4^(n‑2) if n is even,
 *            a(n)=3*4^(n‑2)−2^(n‑3) if n is odd
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp using repeated squaring.  Base and result are of type seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1)
            res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t pow4 = pow_seq(4, n - 2);          /* 4^(n-2) */
    seq_t term = 3 * pow4;                   /* 3*4^(n-2) */

    if (n & 1) { /* odd n, subtract 2^(n-3) */
        seq_t pow2 = pow_seq(2, n - 3);      /* 2^(n-3) */
        term = term - pow2;
    }

    return term;
}
