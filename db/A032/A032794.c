/*
 * A032794  Positive integers of the form
 *          n(n+1)(n+2)(n+3)(n+4)/(n+(n+1)+(n+2)+(n+3)+(n+4)) that are a
 *          multiple of n.
 * https://oeis.org/A032794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * ((n+1)(n+3)(n+4))/5 where n is the n‑th integer with
 *            n%5∈{1,2,4}
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 228, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const idx_t _valid_offset[3] = {1, 2, 4};

seq_t a(idx_t n) {
    /* Convert sequence index n to the underlying integer n0 (≡1,2,4 mod 5) */
    idx_t idx   = n - 1;                 /* 0‑based position in the sequence */
    idx_t block = idx / 3;               /* number of complete 5‑blocks visited */
    idx_t rem   = idx % 3;               /* 0,1,2 maps to 1,2,4 */

    seq_t n0 = (seq_t)block;             /* use seq_t for all arithmetic */
    n0 *= 5;
    n0 += _valid_offset[rem];            /* n0 is the original integer */

/* Compute a(n) = n0 * ((n0+1)(n0+3)(n0+4))/5 */
    seq_t t1 = n0 + 1;
    seq_t t2 = n0 + 3;
    seq_t t3 = n0 + 4;

    seq_t prod = t1 * t2;
    prod = prod * t3;                    /* (n0+1)(n0+3)(n0+4) */
    prod /= 5;                           /* guaranteed integer division */

    return n0 * prod;                    /* final sequence value */
}
