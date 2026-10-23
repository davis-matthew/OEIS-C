/*
 * A023712  Numbers with exactly 3 1's in their base 4 expansion.
 * https://oeis.org/A023712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   three_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023712: numbers with exactly 3 ones in base‑4 */

#include <stdbool.h>
#include "../utils/three_ones.h"

/* Main function requested by the specification */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* not used, but keep defined for safety */

    idx_t seen = 0;
    seq_t x = 0;

    while (seen < n) {
        ++x;
        if (three_ones(x))
            ++seen;
    }
    return x;
}
