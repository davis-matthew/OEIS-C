/*
 * A044312  Numbers n such that string 6,7 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth natural number such that its base‑9
 *            representation contains the consecutive digits 6,7, while
 *            (a(n)-1) does not
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has67_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has67_idx.h"

/* OEIS A044312: numbers n such that base‑9 representation of n
   contains "67" but base‑9 representation of n-1 does not. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; this value is unused */

    idx_t found = 0;
    for (idx_t num = 1;; ++num) {
        if (has67_idx(num) && !has67_idx(num - 1)) {
            ++found;
            if (found == n) return (seq_t)num;
        }
    }
}
