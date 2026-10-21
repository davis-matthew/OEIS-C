/*
 * A064167  Product of numerator and denominator of the n-th harmonic number,
 *          1 + 1/2 + 1/3 +...+ 1/n.
 * https://oeis.org/A064167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = product of numerator and denominator of the n‑th harmonic number,
 *            i.e. if H_n = A_n / B_n in lowest terms, then a(n) = A_n * B_n.
 * The routine computes H_n iteratively, reducing at each step.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1, return 0 as safe default */
        return 0;

    /* Handle the first harmonic number directly.  H_1 = 1/1. */
    if (n == 1)
        return 1;

    seq_t num = 1;  /* numerator A_k of H_k */
    seq_t den = 1;  /* denominator B_k of H_k */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t kseq = (seq_t)k;          /* cast integer k to seq_t */
        seq_t new_num = num * kseq + den;
        seq_t new_den = den * kseq;

        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        num = new_num;
        den = new_den;
    }

    return num * den;
}
