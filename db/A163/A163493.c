/*
 * A163493  Number of binary strings of length n which have the same number of
 *          00 and 01 substrings.
 * https://oeis.org/A163493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} ( binom(2k-1,k)*binom(n-2k,k) * +
 *            binom(2k,k)*binom(n-2k-1,k) ) * with binom(m,k) = 0 if m < k *
 *            (binom(-1,0) = 1) implicitly handled by the k==0 case
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 67, u128 n <= 132,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t total = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = 0;
        seq_t term2 = 0;

        /* first part */
        if (k == 0) {
            /* binom(-1,0) = 1 and binom(n,0) = 1 */
            term1 = (seq_t)1;
        } else if (n >= 3 * k) {          /* ensures n-2k >= k  */
            term1 = binom(2 * k - 1, k) * binom(n - 2 * k, k);
        }

        /* second part */
        if (n >= 2 * k + 1) {            /* no underflow in n-2k-1 */
            idx_t nmk1 = n - 2 * k - 1;
            if (nmk1 >= k) {             /* ensures n-2k-1 >= k  */
                term2 = binom(2 * k, k) * binom(nmk1, k);
            }
        }

        total += term1 + term2;
    }

    return total;
}
