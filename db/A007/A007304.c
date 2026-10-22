/*
 * A007304  Sphenic numbers: products of 3 distinct primes.
 * https://oeis.org/A007304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   Sphenic numbers = products of exactly 3 distinct primes; a(n) is
 *            the n-th such number
 * Range:     u8 n <= 25, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_sphenic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_sphenic.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    
    for (seq_t candidate = 30; ; candidate++) {
        if (is_sphenic(candidate)) {
            if (++count == n) {
                return candidate;
            }
        }
    }
}
