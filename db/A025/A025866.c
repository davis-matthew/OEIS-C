/*
 * A025866  Expansion of 1/((1-x^4)*(1-x^6)*(1-x^11)).
 * https://oeis.org/A025866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+10)*(n+11)/528 + (n+8)*(-1)^n/48 + *
 *            ((8*n^2+3*n+7) mod 11)/11)
 * Range:     u8 n <= 0, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 4, 6, and 11. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic */
    seq_t nval = (seq_t)n;

    /* term1 numerator: (n+10)*(n+11) */
    seq_t t1 = (nval + (seq_t)10) * (nval + (seq_t)11);

    /* term2 numerator multiplied by 11: (n+8)*(-1)^n*11 */
    seq_t t2_pos = (nval + (seq_t)8) * (seq_t)11;

    /* (8*n^2 + 3*n + 7) mod 11 */
    seq_t temp = ((seq_t)8 * nval) % (seq_t)11;
    temp = (temp * nval) % (seq_t)11;
    seq_t modval = (temp + (((seq_t)3 * nval) % (seq_t)11) + (seq_t)7) % (seq_t)11;

    /* term3 numerator: (( ...) mod 11) * 48 */
    seq_t t3 = modval * (seq_t)48;

    /* Sum all numerators, handling the sign of term2 */
    seq_t sum = t1 + t3;
    if ((n & (idx_t)1) == 0) {
        /* n even: add term2 */
        sum += t2_pos;
    } else {
        /* n odd: subtract term2 (negative contribution) */
        sum -= t2_pos;   /* safe: sum > t2_pos for all n */
    }

    /* floor division by the common denominator 528 */
    return sum / (seq_t)528;
}
