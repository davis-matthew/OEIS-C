/*
 * A062919  "Reverse factorials": product of the digit reversals of the
 *          numbers 1 through n.
 * https://oeis.org/A062919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = ∏_{k=1..n} reverse(k) (Reverse factorial)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   reverse_uint64_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/reverse_uint64_local.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t rev = reverse_uint64_local(k);
        result *= (seq_t)rev;
    }
    return result;
}
