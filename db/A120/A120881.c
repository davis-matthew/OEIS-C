/*
 * A120881  a(n) = number of k's, for 1 <= k <= n, where GCD(k,floor(n/k)) >
 *          1.
 * https://oeis.org/A120881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 797, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of k, 1 <= k <= n, for which gcd(k, floor(n/k)) > 1.
 * This function simply iterates over all k and counts those satisfying the
 * condition using the provided gcd helper.
 */
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t m = (seq_t)(n / k);          // floor(n/k)
        if (gcd((seq_t)k, m) > 1) {
            ++count;
        }
    }
    return count;
}
