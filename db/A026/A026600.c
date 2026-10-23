/*
 * A026600  a(n) is the n-th letter of the infinite word generated from w(1)=1
 *          inductively by w(n)=JUXTAPOSITION{w(n-1),w'(n-1),w"(n-1)}, where
 *          w(k) becomes w'(k) by the cyclic permutation 1->2->3->1 and w"(k)
 *          = (w')'(k).
 * https://oeis.org/A026600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of base‑3 digits of (n-1)) mod 3 + 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1; compute the sum of base‑3 digits of n‑1 modulo 3 */
    idx_t m = n - 1;               /* n-1 fits in idx_t (uint64_t) */
    unsigned sum_mod3 = 0;         /* 0,1,2 only */

    while (m != 0) {
        sum_mod3 += (unsigned)(m % 3);
        sum_mod3 %= 3;
        m /= 3;
    }

    /* the sequence values are 1,2,3 */
    return (seq_t)(sum_mod3 + 1);
}
