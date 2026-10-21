/*
 * A238769  Number of nX2 0..3 arrays with no element equal to the sum modulo
 *          4 of elements to its left or elements above it.
 * https://oeis.org/A238769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A238769: Number of n×2 arrays with entries 0..3 such that no element equals
   the sum modulo 4 of the elements to its left or of the elements above it.
   Computed by dynamic programming over rows with state (c1, c2) = cumulative sums
   of columns 1 and 2 modulo 4. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Transition matrix: trans[s][t] = number of ways to go from state s to state t
       where state s / t are encoded as s = c1*4 + c2 (cumulative sums modulo 4). */
    static bool built = false;
    static seq_t trans[16][16];

    if (!built) {
        for (int c1 = 0; c1 < 4; ++c1) {
            for (int c2 = 0; c2 < 4; ++c2) {
                int s = c1 * 4 + c2;
                for (int x1 = 0; x1 < 4; ++x1) {
                    if (x1 == 0 || x1 == c1) continue;          // column 1 constraints
                    for (int x2 = 0; x2 < 4; ++x2) {
                        if (x2 == x1 || x2 == c2) continue;    // column 2 constraints
                        int ns = ((c1 + x1) & 3) * 4 + ((c2 + x2) & 3);
                        trans[s][ns] += 1;
                    }
                }
            }
        }
        built = true;
    }

    seq_t dp_prev[16] = {0};
    seq_t dp_next[16] = {0};
    dp_prev[0] = 1;  // initial cumulative sums (0,0)

    for (idx_t row = 0; row < n; ++row) {
        for (int t = 0; t < 16; ++t) dp_next[t] = 0;
        for (int s = 0; s < 16; ++s) {
            seq_t base = dp_prev[s];
            if (base == 0) continue;
            for (int t = 0; t < 16; ++t) {
                seq_t add = trans[s][t];
                if (add) dp_next[t] += base * add;
            }
        }
        for (int s = 0; s < 16; ++s) dp_prev[s] = dp_next[s];
    }

    seq_t total = 0;
    for (int s = 0; s < 16; ++s) total += dp_prev[s];
    return total;
}
