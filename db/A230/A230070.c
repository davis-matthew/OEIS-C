/*
 * A230070  a(n) is the number of odious integers (A000069) not exceeding n
 *          and respectively prime to n.
 * https://oeis.org/A230070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{k ≤ n : gcd(k,n) = 1 and popcount(k) is odd}|
 * Range:     u8 n <= 520, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   _is_odious_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_is_odious_idx.h"

/* Local helper: gcd for idx_t (uint64_t). */
static inline idx_t _gcd(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t cnt = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (_gcd(k, n) == 1 && _is_odious_idx(k)) {
            ++cnt;
        }
    }
    return cnt;
}
