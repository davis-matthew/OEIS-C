/*
 * A196438  a(n) is the number of integers k <= n such that GCQ_A(n, k) >= 2
 *          (see definition in comments).
 * https://oeis.org/A196438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k ∈ [1,n] | ∃ q with 2 ≤ q ≤ min(n,k) and q does not
 *            divide n and q does not divide k }|
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        bool found = false;
        for (idx_t q = 2; q <= k; ++q) {
            if ((k % q) != 0 && (n % q) != 0) {
                found = true;
                break;
            }
        }
        if (found) {
            ++count;
        }
    }
    return count;
}
