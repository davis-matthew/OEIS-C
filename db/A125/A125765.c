/*
 * A125765  Consider the array T(n, m) = m-th prime of the form n*i(i+1)/2 +/-
 *          1. This sequence is the main diagonal.
 * https://oeis.org/A125765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form n*i(i+1)/2 ± 1 (n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 28, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, so 0 is unused */
    idx_t count = 0;               /* number of primes found so far */

    for (idx_t i = 1;; ++i) {
        /* triangular number T = i(i+1)/2 as seq_t */
        seq_t tri   = (seq_t)i * ((seq_t)i + 1) / 2;
        seq_t base  = (seq_t)n * tri;               /* n * T */

        /* candidate n*T - 1 (skip if < 2) */
        if (base > 1) {
            seq_t cand = base - 1;
            if (is_prime_big(cand)) {
                ++count;
                if (count == n) return cand;
            }
        }

        /* candidate n*T + 1 */
        seq_t cand = base + 1;
        if (is_prime_big(cand)) {
            ++count;
            if (count == n) return cand;
        }
    }
}
