/*
 * A063083  Number of permutations of n elements with an odd number of fixed
 *          points.
 * https://oeis.org/A063083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0 ; a(n)=n*a(n-1)+(-2)^(n-1) for n≥1 */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t a_prev = (seq_t)0;      /* a(0) */
    seq_t pow2   = (seq_t)1;      /* 2^(i-1) for current i */
    for (idx_t i = 1; i <= n; ++i) {
        /* compute i * a_prev */
        seq_t mul = ((seq_t)i) * a_prev;
        seq_t a_i = mul;
        /* add or subtract 2^(i-1) depending on parity of i */
        if ((i & 1) == 0) {          /* even i → subtract */
            a_i = a_i - pow2;
        } else {                     /* odd i → add */
            a_i = a_i + pow2;
        }
        a_prev = a_i;
        pow2   = pow2 * (seq_t)2;    /* prepare next power of two */
    }
    return a_prev;
}
