/*
 * A001118  Number of labeled ordered set partitions into 5 parts for n>=1,
 *          a(0)=1.
 * https://oeis.org/A001118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 5*4^n + 10*3^n - 10*2^n + 5
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"

seq_t a(idx_t n) {
    seq_t pow5 = a000272_ipow((seq_t)5, n);
    seq_t pow4 = a000272_ipow((seq_t)4, n);
    seq_t pow3 = a000272_ipow((seq_t)3, n);
    seq_t pow2 = a000272_ipow((seq_t)2, n);
    
    return pow5 - (seq_t)5 * pow4 + (seq_t)10 * pow3 - (seq_t)10 * pow2 + (seq_t)5;
}
