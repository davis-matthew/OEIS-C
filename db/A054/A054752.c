/*
 * A054752  Number of inequivalent n X n matrices over GF(5) under action of
 *          dihedral group of the square D_4.
 * https://oeis.org/A054752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1/8 * (5^(n²) + 2*5^(n²/4) + 3*5^(n²/2) + 2*5^((n²+n)/2))
 *            for even n; a(n) = 1/8 * (5^(n²) + 2*5^((n²+3)/4) + 5^((n²+1)/2)
 *            + 4*5^((n²+n)/2)) for odd n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow5_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow5_exp.h"

seq_t a(idx_t n) {
    idx_t n_sq = n * n;                 /* n²  (fits in 64 bits for the intended limits) */
    idx_t term2_exp, term3_exp, term4_exp;

    seq_t term1 = pow5_exp(n_sq);

    seq_t sum;
    if ((n & 1) == 0) { /* even n */
        term2_exp = n_sq / 4;
        term3_exp = n_sq / 2;
        term4_exp = (n_sq + n) / 2;

        sum = term1
            + 2 * pow5_exp(term2_exp)
            + 3 * pow5_exp(term3_exp)
            + 2 * pow5_exp(term4_exp);
    } else { /* odd n */
        term2_exp = (n_sq + 3) / 4;
        term3_exp = (n_sq + 1) / 2;
        term4_exp = (n_sq + n) / 2;

        sum = term1
            + 2 * pow5_exp(term2_exp)
            +     pow5_exp(term3_exp)
            + 4 * pow5_exp(term4_exp);
    }

    return sum / 8;   /* division is exact */
}
