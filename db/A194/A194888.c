/*
 * A194888  Number of lower triangles of an n X n 0..3 array with new values
 *          introduced in row major order 0..3 and no two horizontal or
 *          vertical neighbors of any element equal.
 * https://oeis.org/A194888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(2*n-2) - (1-(-1)^n)*21/8*2^(3*n/2+1/2) * -
 *            (1+(-1)^n)*15/4*2^(3*n/2) + 6*2^n * Empirical formula valid for
 *            n >= 3. The values for n = 1,2 are * handled separately
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* small n handled directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)24;

    /* compute 9*2^(2*n-2) */
    idx_t e1 = 2 * n - 2;
    seq_t term1 = 9 * pow2(e1);

    /* compute 6*2^n */
    seq_t term4 = 6 * pow2(n);

    /* result starts as term1 + term4 */
    seq_t result = term1 + term4;

    if (n & 1) {          /* n is odd */
        /*  (1-(-1)^n) = 2  ==> factor 21/4 */
        idx_t exp = (3 * n + 1) / 2;      /* integer for odd n */
        seq_t temp = ((seq_t)21 * pow2(exp)) / 4;
        result -= temp;
    } else {              /* n is even */
        /*  (1+(-1)^n) = 2  ==> factor 15/2 */
        idx_t exp = (3 * n) / 2;          /* integer for even n */
        seq_t temp = ((seq_t)15 * pow2(exp)) / 2;
        result -= temp;
    }

    return result;
}
