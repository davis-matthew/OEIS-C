/*
 * A006099  Gaussian binomial coefficient [ n, n/2 ] for q=2.
 * https://oeis.org/A006099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n, floor(n/2)]_2 = Product_{j=0..k-1}
 *            (2^(n-j)-1)/(2^(j+1)-1), k = floor(n/2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 15, u128 n <= 22,
 *            big any n
 * Helpers:   gb_gcd, gb_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gb_gcd.h"
#include "../utils/gb_pow2.h"

seq_t a(idx_t n) {
    idx_t k = n / 2;
    seq_t result = 1;
    if (k == 0) return result;

    seq_t pn = gb_pow2(n);   /* 2^(n-j) */
    seq_t pd = 2;            /* 2^(j+1) */
    for (idx_t j = 0; j < k; j++) {
        seq_t num = pn - 1;
        seq_t den = pd - 1;
        /* result * num / den is exactly [n, j+1]_2; divide before multiplying */
        seq_t g = gb_gcd(result, den);
        result /= g;
        den /= g;
        num /= den;
        result *= num;
        pn /= 2;
        pd *= 2;
    }
    return result;
}
