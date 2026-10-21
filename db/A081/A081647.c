/*
 * A081647  Integers congruent to 0, 1, 4, 9, 16, 25, 36 or 49 (mod 64) which
 *          are not squares.
 * https://oeis.org/A081647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A081647:
   a(n) = the nth positive integer congruent to 0,1,4,9,16,25,36 or 49 (mod 64)
   that is not a perfect square.  The implementation tests integers in
   increasing order, skipping squares and non‑residues. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Returns the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Bit mask for the admissible residues modulo 64. */
    static const unsigned long long RES_MASK =
        (1ULL << 0) | (1ULL << 1) | (1ULL << 4) |
        (1ULL << 9) | (1ULL << 16) | (1ULL << 25) |
        (1ULL << 36) | (1ULL << 49);

    seq_t val   = 1;   /* Current integer to test. */
    idx_t count = 0;   /* Count of qualifying integers found so far. */

    while (1) {
        unsigned r = (unsigned)(val & 63ULL);
        if (RES_MASK & (1ULL << r)) {
            /* Check if val is a perfect square. */
            seq_t s = floor_sqrt(val);
            if (s * s != val) {
                ++count;
                if (count == n) {
                    return val;
                }
            }
        }
        ++val;
    }
}
