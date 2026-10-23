/*
 * A152018  Denominator of 1/n^2 - 1/(3*n)^2 or of 8/(9*n^2).
 * https://oeis.org/A152018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * n^2 / 2^{min(3, 2*v2(n))}
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here v2(n) is the exponent of 2 in the prime factorisation of n      */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 9 * n^2 */
    seq_t val = (seq_t)n * (seq_t)n;
    val *= (seq_t)9;

    /* count the exponent of 2 in n */
    unsigned k = 0;          /* k = v2(n)            */
    idx_t tmp = n;           /* use a temporary copy   */
    while ((tmp & 1) == 0) {
        ++k;
        tmp >>= 1;
    }

    /* shift right by min(3, 2*k) bits */
    unsigned shift = (k >= 2) ? 3 : (k << 1);  /* 2*k for k=0,1 */
    val >>= shift;

    return val;
}
