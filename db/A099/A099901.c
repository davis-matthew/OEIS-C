/*
 * A099901  Shifts left and divides by 2 under the XOR BINOMIAL transform
 *          (A099902).
 * https://oeis.org/A099901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   a(0) = 1 a(n) = XOR_{k=0..n-1} (C(n-1-k + floor(k/2),
 *            floor(k/2)) mod 2) * 2^(k+1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <limits.h>   // for CHAR_BIT

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    const unsigned int SEQ_BITS = sizeof(seq_t) * CHAR_BIT;
    seq_t result = (seq_t)0;
    for (idx_t k = 0; k < n; ++k) {
        /* a = n-1 - ceil(k/2)  and  b = floor(k/2) */
        idx_t ceil_half = (k + 1) / 2;   /* ceil(k/2) */
        idx_t a = n - 1 - ceil_half;
        idx_t b = k / 2;                /* floor(k/2) */

        /* binomial(a,b) is odd iff (b & ~a) == 0 */
        if ( ((~a) & b) == 0 ) {
            /* set bit (k+1) if it fits in the type */
            if ((unsigned int)(k + 1) < SEQ_BITS) {
                result ^= ((seq_t)1 << (k + 1));
            }
        }
    }
    return result;
}
