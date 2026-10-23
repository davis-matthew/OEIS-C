/*
 * A117804  Natural position of n in the string 12345678910111213....
 * https://oeis.org/A117804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = d*n + 1 - (10^d - 1)/9 * where d is the number of decimal
 *            digits of n
 * Range:     u8 n <= 120, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset starts at 1; n>=1 in practice */

    /* count decimal digits of n */
    idx_t d = 0;
    idx_t tmp = n;
    do {
        ++d;
        tmp /= 10;
    } while (tmp);

    /* compute 10^d  */
    seq_t pow = pow10(d);               /* pow = 10^d  */
    seq_t sub = (pow - (seq_t)1) / (seq_t)9;  /* (10^d-1)/9 */
    seq_t prod = (seq_t)d * (seq_t)n;     /* d*n          */

    return prod + (seq_t)1 - sub;        /* final result */
}
