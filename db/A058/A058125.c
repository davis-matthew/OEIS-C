/*
 * A058125  n^(n^n)/(n^n)^n with 0^0=1.
 * https://oeis.org/A058125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n^n - n^2)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t sn = (seq_t)n;

    /* Compute n^n (uses small exponent) */
    seq_t exp_n = (seq_t)n;
    seq_t n_pow_n = big_pow(sn, exp_n);

    /* Compute n^2 */
    seq_t n_sq = sn * sn;

    /* Exponent for final power: n^n - n^2 */
    seq_t exp = n_pow_n - n_sq;

    /* Final result: n^(exp) */
    return big_pow(sn, exp);
}
