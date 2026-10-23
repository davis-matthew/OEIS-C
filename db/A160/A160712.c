/*
 * A160712  Composite cyclops numbers (A134808).
 * https://oeis.org/A160712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Composite cyclops numbers: A134808 – composite numbers of the form a0b
   where 1 ≤ a,b ≤ 9 (i.e. three‑digit numbers with a zero in the tens
   position and non‑zero units digit).  */
#include "../utils/is_prime_big.h"
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t a_digit = 1; ; ++a_digit) {          /* hundreds digit (1‑9)   */
        seq_t base = (seq_t)a_digit * 100;          /* a00                   */
        for (idx_t b_digit = 1; b_digit <= 9; ++b_digit) { /* units digit 1‑9 */
            seq_t candidate = base + b_digit;       /* a0b                   */
            /* skip primes – keep only composites */
            if (!is_prime_big(candidate)) {
                ++count;
                if (count == n) {
                    return candidate;
                }
            }
        }
    }
}
