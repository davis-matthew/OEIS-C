/*
 * A044243  Numbers n such that string 7,0 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computed by searching for numbers n whose base‑8
 *            representation contains "70" but n-1 does not
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   contains70
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/contains70.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, so n == 0 is undefined */

    idx_t found = 0;
    for (idx_t cand = 1; ; ++cand) {
        seq_t val = (seq_t)cand;
        if (contains70(val) && !contains70(val - (seq_t)1)) {
            ++found;
            if (found == n) return val;
        }
    }
}
