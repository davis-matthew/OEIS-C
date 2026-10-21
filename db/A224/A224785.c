/*
 * A224785  Expansion of (1+4*x+8*x^2-x^3)/((1-x)*(1+x)*(1-3*x^2)).
 * https://oeis.org/A224785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*3^(n/2)-9)/2 if n even, (11*3^((n-1)/2)-3)/2 if n odd
 *            with a(0)=1, a(1)=4, a(2)=12, a(3)=15
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 37, u64 n <= 77, u128 n <= 158,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* provides seq_pow(base,exp) */

seq_t a(idx_t n)
{
    /* initial values for n = 0..3 */
    static const seq_t init[4] = { (seq_t)1, (seq_t)4, (seq_t)12, (seq_t)15 };

    if (n < 4)
        return init[n];

    /* split the parity */
    if ((n & 1) == 0) {         /* even n = 2*k */
        idx_t k = n / 2;
        seq_t p = seq_pow((seq_t)3, k);          /* 3^k */
        return (((seq_t)11 * p - (seq_t)9) / (seq_t)2);
    } else {                    /* odd n = 2*k+1 */
        idx_t k = n / 2;
        seq_t p = seq_pow((seq_t)3, k);          /* 3^k */
        return (((seq_t)11 * p - (seq_t)3) / (seq_t)2);
    }
}
