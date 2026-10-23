/*
 * A030134  a(n)^2 has last digit equal to the sum of the other digits.
 * https://oeis.org/A030134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth integer such that the square a(n)^2 * has last
 *            digit equal to the sum of its remaining digits. * For n ≥ 20 the
 *            terms are given by * a(2m+14) = 10^(2m) + 2·10^m + 3 (m ≥ 3,
 *            even index) * a(2m+15) = 10^(2m) + 5·10^m + 3 (m ≥ 3, odd index)
 *            * This follows from the OEIS description th...
 * Range:     u8 none, u16 n <= 10, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   digit_sum, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/pow10.h"

/* Return the nth term of OEIS sequence A030134 (base 10). */
seq_t a(idx_t n)
{
    /* Store the first 19 terms that are found by brute force. */
    static seq_t small[19];
    static idx_t small_len = 0;
    static idx_t candidate_start = 1;   /* next integer to test */

    if (n <= 19) {
        /* Find the first n terms by scanning integers. */
        while (small_len < n) {
            seq_t cand = (seq_t)candidate_start;
            seq_t sq   = cand * cand;
            seq_t sum  = digit_sum(sq);
            seq_t last = sq % 10;          /* last digit of a^2         */
            if (sum - last == last) {      /* sum of other digits == last */
                small[small_len++] = cand;
            }
            candidate_start++;
        }
        return small[n - 1];
    }

    /* For n ≥ 20 use the closed‑form expressions. */
    /* Compute m such that n = 2m + 14.  Hence m = (n - 20)/2 + 3.   */
    idx_t m = (n - 20) / 2 + 3;
    seq_t pow_m  = pow10(m);          /* 10^m     */
    seq_t pow_2m = pow10(2 * m);      /* 10^(2m)  */
    seq_t coeff  = (n % 2 == 0) ? (seq_t)2 : (seq_t)5;
    return pow_2m + coeff * pow_m + (seq_t)3;
}
