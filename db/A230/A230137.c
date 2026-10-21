/*
 * A230137  a(n)/2^n is the expected value of the maximum of the number of
 *          heads and the number of tails when n fair coins are tossed.
 * https://oeis.org/A230137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = (4^n + binomial(2n,n)) * n Formula: a(2n+1) = (4^n +
 *            binomial(2n,n)) * (2n+1)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* handles the offset term */
    /* even n = 2*m */
    if ((n & 1) == 0) {
        idx_t m = n >> 1;                 /* m = n/2 */
        seq_t p = pow2(n);                /* 2^n = 4^m */
        seq_t bin = binomial(n, m);       /* C(2m, m) */
        return (p + bin) * (seq_t)m;
    } else {                              /* odd n = 2*m+1 */
        idx_t m = (n - 1) >> 1;           /* m = (n-1)/2 */
        seq_t p = pow2(n - 1);            /* 2^(n-1) = 4^m */
        seq_t bin = binomial(n - 1, m);   /* C(2m, m) */
        return (p + bin) * n;
    }
}
