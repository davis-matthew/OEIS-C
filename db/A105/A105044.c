/*
 * A105044  Numbers n such that 1000*n - 1 is prime.
 * https://oeis.org/A105044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: return the n‑th integer m ≥ 2 such that 1000*m − 1 is prime. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; n ≥ 1 in normal use */

    idx_t count   = 0;
    uint64_t m    = 2;               /* candidate integer m */

    for (;; ++m) {
        uint64_t p = 1000ULL * m - 1;  /* the corresponding prime candidate */
        if (is_prime(p)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
