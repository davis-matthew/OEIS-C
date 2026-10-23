/*
 * A047350  Numbers that are congruent to {1, 2, 4} mod 7.
 * https://oeis.org/A047350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 3 - 2*floor(n/3) - 3*floor((n+1)/3)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A047350: Numbers that are congruent to {1,2,4} mod 7. */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;        /* cast index to the sequence type */
    seq_t res = 4 * nn;         /* 4*n */
    res -= 3;                   /* -3 */
    seq_t div1 = nn / 3;        /* floor(n/3) */
    seq_t div2 = (nn + 1) / 3;  /* floor((n+1)/3) */
    res -= 2 * div1;            /* -2*floor(n/3) */
    res -= 3 * div2;            /* -3*floor((n+1)/3) */
    return res;
}
