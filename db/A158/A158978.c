/*
 * A158978  a(n) = product of numbers k <= n such that not all proper divisors
 *          of k are divisors of n.
 * https://oeis.org/A158978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all k ≤ n for which at least one proper
 *            divisor of k does not divide n. (k is excluded when all its
 *            proper divisors divide n.)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        bool include = false;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                if (d < k && (n % d != 0)) {
                    include = true;
                    break;
                }
                idx_t d2 = k / d;
                if (d2 < k && d2 != d && (n % d2 != 0)) {
                    include = true;
                    break;
                }
            }
        }
        if (include) {
            result = result * (seq_t)k;
        }
    }
    return result;
}
