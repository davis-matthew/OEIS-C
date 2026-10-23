/*
 * A028298  Triangle of coefficients in expansion of sin(n*x) (or
 *          sin(n*x)/cos(x) if n is even) in ascending powers of sin(x).
 * https://oeis.org/A028298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf, nice, easy
 * Formula:   T(r,k) = (-1)^k * ((r-2k)(-1)^r - r - 2k) / (2r + (-1)^r - 1 +
 *            4k) * 2^(2k+(((-1)^r-1)/2)) * binomial((2r+(-1)^r-1)/4 + k,
 *            (2r-(-1)^r+1)/4 - k) The sequence a(n) gives the terms of this
 *            triangle read left‑to‑right, top‑to‑bottom (r = 1,2,3,…, k =
 *            1,…,⌈r/2⌉)
 * Range:     i8 n <= 7, i16 n <= 28, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   binomial, idx_to_rc, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/idx_to_rc.h"

seq_t a(idx_t n)
{
    idx_t r, k;
    idx_to_rc(n, &r, &k);

    int sign_r = (r % 2 == 0) ? 1 : -1;   /* (-1)^r  */
    int sign_k = (k % 2 == 0) ? 1 : -1;   /* (-1)^k  */

    /* terms of the numerator that are potentially negative   */
    int64_t diff = (int64_t)r - 2 * (int64_t)k;               /* r-2k   */
    seq_t num1 = (seq_t)(diff * sign_r) - (seq_t)r -
                 (seq_t)(2 * (int64_t)k);                    /* ((r-2k)(-1)^r - r - 2k) */

    /* denominator */
    int64_t denom_int =
        2 * (int64_t)r + sign_r - 1 + 4 * (int64_t)k;        /* 2r+(-1)^r-1+4k */
    seq_t denom = (seq_t)denom_int;

    /* power of 2 factor */
    idx_t exp = 2 * k + ((sign_r - 1) / 2);                 /* 2k + ((-1)^r-1)/2 */
    seq_t pow2_val = pow2(exp);

    /* binomial coefficient */
    idx_t a_bin_tmp = ((2 * r + sign_r - 1) / 4);
    idx_t b_bin_tmp = ((2 * r - sign_r + 1) / 4);
    idx_t a_bin = a_bin_tmp + k;
    idx_t b_bin = b_bin_tmp - k;
    seq_t bin = binomial(a_bin, b_bin);

    /* full numerator with sign_k factor */
    seq_t numerator = (seq_t)sign_k;
    numerator *= num1;
    numerator *= pow2_val;
    numerator *= bin;

    return numerator / denom;
}
