/*
 * A196437  a(n) = the number of numbers k <= n such that GCQ_A(n, k) =
 *          LCQ_A(n, k) = 0 (see definition in comments).
 * https://oeis.org/A196437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k ≤ n such that for every integer q with 1≤q≤k,
 *            either q divides k or q divides n. This ensures
 *            GCQ_A(n,k)=LCQ_A(n,k)=0
 * Range:     u8 n <= 9934, u16 n <= 9934, u32 n <= 9934, u64 n <= 9934,
 *            u128 n <= 9934, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        bool ok = true;
        for (idx_t q = 1; q <= k; ++q) {
            if (n % q == 0) continue;      /* q divides n – fine */
            if (k % q != 0) {               /* q divides neither k nor n */
                ok = false;
                break;
            }
        }
        if (ok) ++count;
    }
    return count;
}
