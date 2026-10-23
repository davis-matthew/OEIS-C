/*
 * A138664  a(n) = number of positive integers k, k <= n, where the number of
 *          one's in the binary representation of each k divides n.
 * https://oeis.org/A138664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of k ≤ n such that popcount(k) divides n
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        unsigned int ones = __builtin_popcountll(k);
        /* All k > 0 have ones > 0, but guard anyway */
        if (ones == 0) continue;
        seq_t ones_s = (seq_t)ones;
        if ((seq_t)n % ones_s == 0) {
            count += 1;
        }
    }
    return count;
}
