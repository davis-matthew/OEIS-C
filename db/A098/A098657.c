/*
 * A098657  Expansion of (1-x-4x^2)/((1-2x)(1-8x^2)).
 * https://oeis.org/A098657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n+1)=4^n; a(2n)=(8^n+4^n)/2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute 2^exp as a seq_t; exp fits in idx_t */
static seq_t pow2(idx_t exp)
{
    seq_t result = 1;
    seq_t base = 2;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n is unsigned; use bit operations where convenient */
    if ((n & 1) == 0) {          /* even index: a(2k) */
        idx_t k = n >> 1;        /* k = n/2 */
        seq_t pow2k = pow2(k);
        seq_t pow4k = pow2k * pow2k;     /* 4^k */
        seq_t pow8k = pow4k * pow2k;     /* 8^k */
        seq_t sum = pow8k + pow4k;       /* 8^k + 4^k */
        return sum / 2;                    /* (8^k + 4^k) / 2 */
    } else {                         /* odd index: a(2k+1) = 4^k */
        idx_t k = n >> 1;          /* k = (n-1)/2 */
        seq_t pow2k = pow2(k);
        seq_t pow4k = pow2k * pow2k;     /* 4^k */
        return pow4k;
    }
}
