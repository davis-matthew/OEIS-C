/*
 * A217768  Smallest number k > 0 for which sigma(k - n) = sigma(k + n).
 * https://oeis.org/A217768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A217768: a(n) = smallest k > 0 such that sigma(k − n) = sigma(k + n)
 * where sigma(m) is the sum‑of‑divisors function and sigma(−m) = sigma(m).
 *
 * The computation searches k = 1, 2, 3, … until the equality holds.
 * No additional memory allocation is required other than temporary
 * variables for the sigma values.
 */

#include "../utils/sigma_of.h"   /* provides seq_t sigma_of(idx_t n) */

seq_t a(idx_t n)
{
    /* For n >= 1, find the least positive k with sigma(k−n) == sigma(k+n). */
    for (idx_t k = 1; ; ++k) {
        /* |k - n| */
        idx_t d1 = (k > n) ? (k - n) : (n - k);
        seq_t s1 = sigma_of(d1);

        /* k + n  (always positive) */
        idx_t d2 = k + n;
        seq_t s2 = sigma_of(d2);

        if (s1 == s2) {
            return (seq_t)k;
        }
    }
}
