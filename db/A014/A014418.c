/*
 * A014418  Representation of n in base of Catalan numbers (a classic greedy
 *          version).
 * https://oeis.org/A014418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 131, u32 n <= 9909, u64 n <= 9909,
 *            u128 n <= 9909, big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy Catalan Base representation for OEIS A014418.
   For n ≥ 0 the function returns the decimal number
   a(n) whose digits d_k (k ≥ 1, least significant digit
   corresponds to C(1)=1) satisfy
          n = Σ d_k · C(k)   with 0 ≤ d_k ≤ 3.
   The representation is obtained greedily: choose the largest
   k such that C(k) ≤ remaining n, take the largest possible digit
   (capped at 3), subtract and continue.  The digits are concatenated
   in decimal order to form the result. */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Special case: a(0) = 0. */
    if (n == 0)
        return 0;

    seq_t rem = (seq_t)n;      /* Remaining value to represent   */
    idx_t max_k = 0;           /* Highest index k with C(k) ≤ rem*/

    /* Find largest k with Catalan(k) <= rem. */
    for (idx_t k = 1; ; ++k) {
        seq_t c = catalan(k);
        if (c == 0 /* overflow or unset */ || c > rem)
            break;
        max_k = k;
    }

    seq_t res = 0;           /* Result being built */

    /* Build the decimal digits from most significant k down to 1. */
    for (idx_t k = max_k; k > 0; --k) {
        seq_t c = catalan(k);
        seq_t digit = rem / c;
        if (digit > 3)
            digit = 3;       /* digits can never exceed 3 */
        rem -= digit * c;
        res = res * 10 + digit;
    }

    return res;
}
