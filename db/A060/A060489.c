/*
 * A060489  Number of 5-block ordered tricoverings of an unlabeled n-set.
 * https://oeis.org/A060489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+9,9) - 15*binomial(n+3,3) + 45*(n+1) - 40 (the
 *            term 9*binomial(n-1,-1) is always zero)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 43, u64 n <= 568, u128 n <= 1000,
 *            big any n
 * Helpers:   binom128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom128.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* sequence is defined for n >= 1 */

    /* Intermediate results are kept in signed 128‑bit integers to avoid
       overflow and to allow the subtraction of a larger term from a
       smaller one without wrap‑around. */
    __int128 b1 = (__int128)binom128(n + 9, 9);   /* C(n+9,9)   */
    __int128 b2 = (__int128)binom128(n + 3, 3);   /* C(n+3,3)   */

    __int128 res = b1
                   - 15 * b2
                   + 45 * (__int128)(n + 1)
                   - 40;

    return (seq_t)res;
}
