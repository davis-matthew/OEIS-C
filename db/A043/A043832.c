/*
 * A043832  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 0 mod 5.
 * https://oeis.org/A043832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer k such that the number of runs in the base‑4
 *            representation of k is congruent to 0 modulo 5
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   runcount_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/runcount_base4.h"

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t k = 0;
    while (found < n) {
        if (runcount_base4(k) % 5 == 0) {
            found++;
            if (found == n)
                return k;
        }
        k++;
    }
    return 0; /* unreachable */
}
