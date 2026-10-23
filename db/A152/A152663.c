/*
 * A152663  Number of leading odd entries in all permutations of {1,2,...,n}
 *          (see example).
 * https://oeis.org/A152663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n+1) = (2n+1)! Formula: a(2n) = n * (2n)! / (n+1) with n =
 *            n/2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  The function is undefined for n == 0. */
    if ((n & 1U) == 1U) {          /* odd index: a(n) = n! */
        return factorial(n);
    } else {                       /* even index: a(n) = n/2 * (n)! / (n/2+1)  */
        idx_t k = n / 2;           /* k = n/2  */
        seq_t prod = 1;
        /* Compute (n)! / (k+1) by omitting the multiplier (k+1) from the
           product 1·2·…·n.  The division is exact because (k+1) divides n!. */
        for (idx_t i = 1; i <= n; ++i) {
            if (i == k + 1) continue;          /* skip factor (k+1) */
            prod *= (seq_t)i;
        }
        prod *= (seq_t)k;                       /* multiply by k */
        return prod;
    }
}
