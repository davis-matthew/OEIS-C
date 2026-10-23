/*
 * A182649  Algorithm: x->[rx] if x even, x->[rx-x] if x odd, where r =
 *          (1+sqrt(5))/2, []=floor; a(n) is the number of steps from x=n to
 *          0.
 * https://oeis.org/A182649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps required to reach 0 following the
 *            iteration x → ⌊φ·x⌋ if x is even, x → ⌊x/φ⌋ if x is odd where φ
 *            = (1+√5)/2 (the golden ratio)
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* cur holds the current value in the algorithm and steps counts
       the number of performed transitions.  All arithmetic is done in
       seq_t as required. */
    seq_t cur      = (seq_t)n;
    seq_t steps    = 0;

    while (cur != 0) {
        /* We need s = ⌊√(5·cur²)⌋, which equals ⌊cur·√5⌋.
           All arithmetic stays in seq_t, without overflow for
           the values encountered in the 64‑bit build (cur stays
           below 2·10⁹ for the tested range). */
        seq_t prod = (seq_t)5 * cur;   /* 5·cur  */
        prod *= cur;                  /* 5·cur² */
        seq_t s = floor_sqrt(prod);

        if ((cur & (seq_t)1) == 0) {  /* even */
            cur = (cur + s) / 2;
        } else {                      /* odd */
            cur = (s - cur) / 2;
        }
        ++steps;
    }

    return steps;
}
