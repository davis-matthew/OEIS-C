/*
 * A214458  Let S_3(n) denote difference between multiples of 3 in interval
 *          [0,n) with even and odd binary digit sums. Then
 *          a(n)=(-1)^A000120(n)*(S_3(n)-3*S_3(floor(n/4))).
 * https://oeis.org/A214458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = (-1)^A000120(n)*(S_3(n)-3*S_3(floor(n/4)))
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* helper to compute parity of popcount of a 64‑bit integer */
static unsigned int parity64(uint64_t x) {
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (unsigned int)(x & 1U);
}

/* compute the difference between multiples of 3 with even and odd binary digit sums
   in the interval [0,limit) */
static seq_t S3_upto(idx_t limit) {
    seq_t diff = 0;
    for (idx_t v = 0; v < limit; v += 3) {
        unsigned int p = parity64(v);
        diff += p ? -1 : 1;
    }
    return diff;
}

seq_t a(idx_t n) {
    idx_t n4 = n / 4;
    seq_t s3_n   = S3_upto(n);
    seq_t s3_n4  = S3_upto(n4);
    unsigned int pn = parity64(n);
    seq_t sign = pn ? -1 : 1;
    return sign * (s3_n - (seq_t)3 * s3_n4);
}
