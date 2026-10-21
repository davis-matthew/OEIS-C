/*
 * A067076  Numbers k such that 2*k + 3 is a prime.
 * https://oeis.org/A067076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer k ≥ 0 such that 2*k + 3 is prime
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    idx_t count = 0;                      /* how many k we have found */
    idx_t k = 0;                          /* candidate k */
    while (count < n) {
        /* candidate = 3 + 2*k */
        seq_t candidate = 3 + ((seq_t)k) * 2;
        if (is_prime_big(candidate)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
    /* This line should never be reached */
    return (seq_t)0;
}
