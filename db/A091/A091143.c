/*
 * A091143  Number of Pythagorean triples mod 2^n; i.e., number of solutions
 *          to x^2 + y^2 = z^2 mod 2^n.
 * https://oeis.org/A091143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = (2^(k+1)-1) * 2^(3k) ; a(2k-1) = (2^k-1) * 2^(3k-1)
 *            (OEIS A091143)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow2(idx_t exp) {
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) {
        r += r;  /* r *= 2 */
    }
    return r;
}

seq_t a(idx_t n) {
    if ((n & 1u) == 0) {            /* n even : n = 2k */
        idx_t k = n >> 1;
        seq_t p1 = pow2(k + 1) - 1; /* 2^(k+1) - 1 */
        seq_t p2 = pow2(3 * k);     /* 2^(3k)    */
        return p1 * p2;
    } else {                         /* n odd  : n = 2k-1 */
        idx_t k = (n + 1) >> 1;
        seq_t p1 = pow2(k) - 1;     /* 2^k - 1      */
        seq_t p2 = pow2(3 * k - 1); /* 2^(3k-1)     */
        return p1 * p2;
    }
}
