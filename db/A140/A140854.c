/*
 * A140854  Primes of the form 210k + 61.
 * https://oeis.org/A140854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes of the form 210k + 61 */
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        seq_t cand = (seq_t)210 * k + (seq_t)61;
        if (is_prime_big(cand)) {
            ++count;
            if (count == n) {
                return cand;
            }
        }
        ++k;
    }
    return 0; /* should never reach here for n >= 1 */
}
