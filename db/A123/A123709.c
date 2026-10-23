/*
 * A123709  a(n) is the number of nonzero elements in row n of triangle
 *          A123706.
 * https://oeis.org/A123709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + #{ k in 1..n-1 | Moebius(n,k+1) <> Moebius(n,k) }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: count transitions of Möbius values for k = 1..n */

/* Include the μ–function from the utilities */
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* n is at least 1 (offset = 1) */
    seq_t first = mobius(n);          /* μ(n) – value at k = 1 */
    seq_t transitions = 0;
    seq_t prev = 0;                   /* μ(0) is treated as 0 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t val = (n % k == 0) ? mobius(n / k) : 0;
        if (val != prev) {
            transitions++;           /* a new transition has occurred */
        }
        prev = val;
    }

    /* The definition counts only transitions between consecutive
       indices 1..n.  The loop above also counts the artificial
       transition from μ(0)=0 to μ(1)=μ(n).  Subtract that if μ(n)≠0. */
    if (first != 0) {
        transitions -= 1;
    }
    return transitions + 1;          /* add the +1 of the definition */
}
