/*
 * A051172  T(n,7), array T as in A051168; a count of Lyndon words; aperiodic
 *          necklaces with 7 black beads and n-7 white beads.
 * https://oeis.org/A051172
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/n) * Σ_{d|gcd(n,7)} μ(d) * C(n/d, 7/d) For n not
 *            divisible by 7 this reduces to a(n) = C(n,7)/n. For n divisible
 *            by 7 this reduces to a(n) = (C(n,7) - n/7)/n
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binom_n_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_n_7.h"

seq_t a(idx_t n)
{
    if (n < 7) return 0;

    seq_t bin = binom_n_7(n);

    if (n % 7 == 0) {
        /* subtract the term for d=7 (μ(7) = -1) */
        seq_t sub = (seq_t)(n / 7);      /* C(n/7,1) = n/7 */
        bin = bin - sub;
    }

    return bin / (seq_t)n;               /* division is exact */
}
