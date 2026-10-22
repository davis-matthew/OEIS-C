/*
 * A006005  The odd prime numbers together with 1.
 * https://oeis.org/A006005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000040(n) - ((-1)^A000040(n)+1)/2
 * Range:     u8 n <= 54, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)1;
    }
    
    idx_t count = 1;
    idx_t candidate = 3;
    
    while (true) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate += 2;
    }
}
