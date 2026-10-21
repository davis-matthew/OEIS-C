/*
 * A081236  Greatest k<n with mu(n-k)=mu(n)=mu(n+k), where mu=A008683 (Moebius
 *          function).
 * https://oeis.org/A081236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest k<n such that mu(n-k)=mu(n)=mu(n+k) (mu = Möbius
 *            function)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n) {
    /* For n=1 the answer is 0 per the OEIS data. */
    if (n <= 1) return (seq_t)0;

    /* Compute Möbius value at n once. */
    seq_t mn = mobius(n);

    /* Scan k from n‑1 down to 1, returning the first that matches the three
       Möbius values.  If no such k exists, return 0. */
    for (idx_t k = n - 1; k > 0; --k) {
        if (mobius(n - k) == mn && mobius(n + k) == mn) {
            return (seq_t)k;
        }
    }
    return (seq_t)0;
}
