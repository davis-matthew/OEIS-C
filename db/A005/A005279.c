/*
 * A005279  Numbers having divisors d, e with d < e < 2*d.
 * https://oeis.org/A005279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers with divisors d, e such that d < e < 2*d
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   collect_divisors, has_divisor_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/collect_divisors.h"
#include "../utils/has_divisor_pair.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 1;
    
    while (count < n) {
        m++;
        if (has_divisor_pair(m)) {
            count++;
        }
    }
    
    return m;
}
