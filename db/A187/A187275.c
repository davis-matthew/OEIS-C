/*
 * A187275  a(n) = (n/4)*5^(n/2)*((1+sqrt(5))^2+(-1)^n*(1-sqrt(5))^2).
 * https://oeis.org/A187275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 6*n*5^n, a(2*n+1) = (2*n+1)*5^(n+1)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 49, u128 n <= 103,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Compute OEIS sequence A187275 */
seq_t a(idx_t n)
{
    /* pow5: 5^(exponent) */
    seq_t pow5;
    if ((n & 1) == 0) {                /* even n */
        idx_t k = n >> 1;              /* n = 2*k */
        pow5 = big_pow((seq_t)5, (seq_t)k);
        return pow5 * ((seq_t)6 * (seq_t)k); /* 6*k*5^k */
    } else {                            /* odd n */
        /* exponent = (n+1)/2  */
        pow5 = big_pow((seq_t)5, (seq_t)((n + 1) >> 1));
        return pow5 * (seq_t)n;          /* n*5^((n+1)/2) */
    }
}
