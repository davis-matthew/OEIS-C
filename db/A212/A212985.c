/*
 * A212985  Number of (w,x,y) with all terms in {0,...,n} and 3w = 3x + y.
 * https://oeis.org/A212985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor(n/3)+1)*(n+1) - floor(n/3)*(floor(n/3)+1)/2
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of (w,x,y) in {0,…,n} satisfying 3w = 3x + y. */

#include <stdint.h>  // for uint64_t, if needed

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t s = nseq / (seq_t)3;            // floor(n/3)
    seq_t splus = s + (seq_t)1;
    seq_t first = splus * (nseq + (seq_t)1);
    seq_t second = s * splus / (seq_t)2;
    return first - second;
}
