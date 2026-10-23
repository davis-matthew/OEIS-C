/*
 * A032870  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^i has d(m)
 *          >= d(m-1) <= d(m-2) >= ...
 * https://oeis.org/A032870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   passes_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032870
   Numbers whose base‑7 representation
   d(m) >= d(m-1) <= d(m-2) >= d(m-3) ...  (alternating inequalities)
   The function searches integers in order and returns the n‑th such number. */

#include <stdbool.h>
#include "../utils/passes_pattern.h"

seq_t a(idx_t n) {
    idx_t cnt = 0;
    seq_t cand = 1;
    while (1) {
        if (passes_pattern(cand)) {
            ++cnt;
            if (cnt == n) return cand;
        }
        ++cand;
    }
}
