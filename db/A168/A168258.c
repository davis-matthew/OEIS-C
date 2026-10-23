/*
 * A168258  Triangle read by rows, A101688 * A000012 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A168258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = min( j , floor((t+2)/2) )
 * where
 *   t = floor( (-1 + sqrt(8*n-7)) / 2 )
 *   j = (t*t + 3*t + 4) / 2 - n
 *
 * All arithmetic is performed using integer types only.
 */

#include <stdint.h>

/* Integer square root of a 64-bit unsigned integer.
 * Returns floor(sqrt(x)).
 */
static idx_t int_sqrt(idx_t x) {
    idx_t result = 0;
    idx_t bit = 1ULL << 62;            /* largest power of 4 <= 2^64 */

    /* Move bit down to the largest power of 4 <= x */
    while (bit > x) bit >>= 2;

    while (bit != 0) {
        if (x >= result + bit) {
            x -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Step 1: compute t = floor( (-1 + sqrt(8*n-7)) / 2 ) */
    idx_t tmp = 8 * n - 7;
    idx_t s   = int_sqrt(tmp);       /* floor(sqrt(8*n-7)) */
    idx_t t   = (s - 1) / 2;         /* floor((s-1)/2) */

    /* Step 2: compute j = (t*t + 3*t + 4)/2 - n */
    seq_t t_seq = (seq_t)t;
    seq_t j = (t_seq * t_seq + 3 * t_seq + 4) / 2 - (seq_t)n;

    /* Step 3: compute k = floor((t+2)/2) */
    seq_t k = (t_seq + 2) / 2;

    /* Step 4: take the minimum of j and k */
    return (j < k) ? j : k;
}
