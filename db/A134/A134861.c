/*
 * A134861  Wythoff BAA numbers.
 * https://oeis.org/A134861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B(A(A(n))) , where A(k) = ⌊k·φ⌋ and B(k)=A(k)+k * The
 *            sequence is Wythoff BAA numbers (OEIS A134861)
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 5446, u64 n <= 9875,
 *            u128 n <= 9875, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

static inline seq_t lower_wythoff(seq_t k) {
    /* A(k) = floor(k*φ) = floor((k + sqrt(5*k^2))/2)  */
    seq_t kk2 = k * k;                     /* k^2          */
    seq_t five_kk2 = kk2 * 5;              /* 5*k^2        */
    seq_t s = floor_sqrt(five_kk2);        /* floor(sqrt(5*k^2)) */
    return (k + s) >> 1;                   /* (k + s)/2   */
}

seq_t a(idx_t n) {
    seq_t t1 = lower_wythoff((seq_t)n);            /* A(n)         */
    seq_t t2 = lower_wythoff(t1);                  /* A(A(n))      */
    seq_t t3 = lower_wythoff(t2);                  /* A(A(A(n)))   */
    return t3 + t2;                               /* B(A(A(n))) = A(A(A(n))) + A(A(n)) */
}
