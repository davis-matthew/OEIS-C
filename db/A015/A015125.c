/*
 * A015125  Triangle of q-binomial coefficients for q=-12.
 * https://oeis.org/A015125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = [s choose k]_{q=-12} with * s = floor((sqrt(8*n+1)-1)/2)
 *            and k = n - s*(s+1)/2 * (q-binomial coefficient with q = -12,
 *            read by antidiagonals)
 * Range:     i8 n <= 3, i16 n <= 15, i32 n <= 17, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Determine the diagonal index s and the position k
       within the diagonal.  The first n terms correspond to
       T[s,k] where s = ⌊(√(8n+1)-1)/2⌋ and k = n - s(s+1)/2. */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) {
        ++s;
    }
    idx_t start = s * (s + 1) / 2;
    idx_t k = n - start;               /* 0 ≤ k ≤ s */

    if (k == 0) {
        return (seq_t)1;                /* [s choose 0]_q = 1 */
    }

    const seq_t q = (seq_t)-12;        /* the value of q */

    /* pow_num holds (-12)^{s+1-i} during the loop;
       initially it is (-12)^s. */
    seq_t pow_num = (seq_t)1;
    for (idx_t i = 0; i < s; ++i) {
        pow_num *= q;                   /* compute (-12)^s */
    }

    /* pow_den holds (-12)^i during the loop; starts at (-12)^1. */
    seq_t pow_den = q;

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)1 - pow_num; /* 1 - (-12)^{s+1-i} */
        seq_t den = (seq_t)1 - pow_den; /* 1 - (-12)^i     */
        res = res * num / den;          /* integer division is exact */

        pow_num /= q;                   /* move to next numerator power */
        pow_den *= q;                   /* move to next denominator power */
    }

    return res;
}
