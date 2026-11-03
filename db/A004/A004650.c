/*
 * A004650  a(n) = prime(n) mod (n+1).
 * https://oeis.org/A004650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) mod (n+1)
 * Range:     u8 n <= 344, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 88 terms through n = 866 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 1;
    
    while (count < n) {
        candidate++;
        if (is_prime_u64(candidate)) {
            count++;
        }
    }
    
    return (seq_t)(candidate % (n + 1));
}
