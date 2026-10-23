/*
 * A165686  Dimension of the space of Siegel cusp forms of genus 2 and weight
 *          2k which are not Saito-Kurokawa lifts of forms of genus 1.
 * https://oeis.org/A165686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+24*n^2-324*n+1080)/1080 + (12-n)*((n mod 2)/24
 *            + ((2*n^2+2*n) mod 3)/18)) - [n<2]
 * Range:     u8 none, u16 n <= 35, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1)
        return 0; /* undefined for n<1, return 0 */

    seq_t nn = (seq_t)n;

    /* numerator of the first term with denominator 1080 */
    seq_t term1_num = nn*nn*nn + (seq_t)24*nn*nn - (seq_t)324*nn + (seq_t)1080;

    /* compute the coefficient part (denominator 1080) */
    uint64_t mod2 = (uint64_t)n & 1ULL;                /* n mod 2 */
    seq_t two_n_sq_plus_two_n = ((seq_t)2*nn*nn) + (seq_t)2*nn;
    uint64_t mod3 = (uint64_t)(two_n_sq_plus_two_n % 3ULL); /* (2*n^2+2*n) mod 3 */

    seq_t coeff = (seq_t)45 * (seq_t)mod2 + (seq_t)60 * (seq_t)mod3;

    seq_t offset = (seq_t)12 - nn;                       /* 12-n */
    seq_t term2_num = offset * coeff;                    /* (12-n)*coeff */

    seq_t total_num = term1_num + term2_num;             /* combined numerator */

    seq_t res = total_num / (seq_t)1080;                 /* floor division */

    if (n < 2)
        res -= 1;                                       /* subtract [n<2] */

    return res;
}
