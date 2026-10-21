/*
 * A089593  Numbers k such that k^2 + 2k + 2 is prime.
 * https://oeis.org/A089593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 43, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A089593 – Numbers k such that k^2 + 2k + 2 is prime.
 * Computation: linear search.  Find the n‑th k (starting with k=0)
 * for which k^2+2k+2 is prime.  No memoisation is needed; a
 * straightforward search is fast enough for n up to 10 000. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* offset 1: a(1) = 0 (k=0 gives 2 which is prime) */
    if (n < 1) return 0;

    seq_t count = 0;
    for (seq_t k = 0;; ++k) {
        seq_t m = k * k + (seq_t)2 * k + (seq_t)2;
        if (is_prime_big(m)) {
            ++count;
            if (count == n) return k;
        }
    }
}
