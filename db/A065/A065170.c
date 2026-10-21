/*
 * A065170  Permutation t->t-3 of Z, folded to N.
 * https://oeis.org/A065170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = f(g(n)-3) where f(z) = 2*z if z>0 = 2*|z|+1 if z<=0 g(n)
 *            = n/2 if n even = (1-n)/2 if n odd
 * Range:     u8 n <= 250, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute g(n) using signed arithmetic */
    int64_t g;
    if ((n & 1) == 0) {
        g = (int64_t)(n >> 1);                 /* even case: n/2 */
    } else {
        g = (int64_t)(1 - (int64_t)n) / 2;     /* odd case: (1-n)/2 */
    }

    /* k = g(n)-3 */
    int64_t k = g - 3;

    /* Apply f(k) */
    if (k > 0) {
        return (seq_t)k * (seq_t)2;
    } else {
        seq_t abs_k = (seq_t)(-k);
        return abs_k * (seq_t)2 + (seq_t)1;
    }
}
