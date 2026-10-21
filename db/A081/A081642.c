/*
 * A081642  Integers congruent to 0, 1 or 4 (mod 9) which are not squares.
 * https://oeis.org/A081642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   isqrt_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081642: Integers congruent to 0, 1 or 4 (mod 9) which are not squares.
   a(n) – the n-th positive integer m ≥ 1 such that m % 9 ∈ {0,1,4} and m is not a perfect square. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_seq_t.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = 1;

    for (;;) {
        seq_t r = m % (seq_t)9;
        /* Accept only numbers with remainder 0, 1, or 4 modulo 9. */
        if (r == 0 || r == 1 || r == 4) {
            seq_t s = isqrt_seq_t(m);
            /* Reject perfect squares. */
            if (s * s != m) {
                ++count;
                if (count == n) {
                    return m;
                }
            }
        }
        ++m;                       /* proceed to next integer */
    }
}
