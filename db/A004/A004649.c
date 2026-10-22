/*
 * A004649  a(n) = prime(n) mod (n-1).
 * https://oeis.org/A004649
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) mod (n-1)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t candidate = 2;
    
    while (1) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                return ((seq_t)candidate) % ((seq_t)(n - 1));
            }
        }
        candidate++;
    }
}
