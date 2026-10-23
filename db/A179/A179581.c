/*
 * A179581  Number of permutations of {1, ..., n+6} with deficiencies #{i :
 *          p(i) < i} exceeding exceedances #{i : p(i) > i} by n or more.
 * https://oeis.org/A179581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 729*3^n + 8*(n+6)*(n-3)*2^n + (n+5)*(n+4)*(n-17)*(n+6)/24
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 34, u128 n <= 50,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* compute powers of 2 and 3 */
    seq_t pow2_n = pow2(n);
    seq_t pow3_n = pow3(n);

    /* leading term 729*3^n */
    seq_t sum = (seq_t)729 * pow3_n;

    /* magnitude of the 8*(n+6)*(n-3)*2^n term (use abs(n-3)) */
    seq_t abs_n_minus_3 = (n >= 3) ? (seq_t)(n - 3) : (seq_t)(3 - n);
    seq_t part2_mag = (seq_t)8 * (seq_t)(n + 6) * abs_n_minus_3 * pow2_n;

    /* magnitude of the (n+5)*(n+4)*(n-17)*(n+6)/24 term (use abs(n-17)) */
    seq_t abs_n_minus_17 = (n >= 17) ? (seq_t)(n - 17) : (seq_t)(17 - n);
    seq_t part3_num = (seq_t)(n + 5) * (seq_t)(n + 4) * (seq_t)(n + 6) * abs_n_minus_17;
    seq_t part3_mag = part3_num / (seq_t)24;

    /* apply signs */
    if (n < 17) {
        sum -= part3_mag;          /* third term is negative */
    } else {
        sum += part3_mag;          /* third term is positive */
    }

    if (n < 3) {
        sum -= part2_mag;          /* second term is negative */
    } else {
        sum += part2_mag;          /* second term is positive */
    }

    return sum;
}
