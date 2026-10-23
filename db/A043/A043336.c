/*
 * A043336  Numbers having four 0's in base 4.
 * https://oeis.org/A043336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   comb_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A043336: Numbers having four 0's in base 4.
 * Formula: a(n) = the n‑th smallest integer whose base‑4 representation
 * contains exactly four zeros.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/comb_uint.h"

/* 3^e  (e is small, ≤ few dozen) */
static inline seq_t pow3_idx(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 3;
    return r;
}

seq_t a(idx_t n)
{
    /* Find the length L of the base-4 number that contains
       exactly four zeros.  The minimal length is 5 (four zeros
       plus a non‑zero leading digit). */
    idx_t L = 5;
    seq_t cumulative = 0;
    seq_t target = (seq_t)n;

    for (;; ++L) {
        seq_t count = comb_uint(L - 1, 4) * pow3_idx(L - 4);
        if (target <= cumulative + count)
            break;
        cumulative += count;
    }

    /* Rank of the desired number within the block of length L
       (1‑based). */
    seq_t rank = target - cumulative;

    /* Unrank the number digit by digit. */
    idx_t zeros_left = 4;
    bool leading = true;
    seq_t result = 0;

    for (idx_t pos = 0; pos < L; ++pos) {
        int d_start = leading ? 1 : 0;
        for (int d = d_start; d <= 3; ++d) {
            seq_t count;
            idx_t rem = L - pos - 1;          /* digits left after this one */
            if (d == 0) {
                if (zeros_left == 0) {
                    count = 0;
                } else {
                    idx_t need = zeros_left - 1;
                    if (need > rem)
                        count = 0;
                    else
                        count = comb_uint(rem, need) * pow3_idx(rem - need);
                }
            } else { /* d = 1, 2, or 3 */
                if (zeros_left > rem)
                    count = 0;
                else
                    count = comb_uint(rem, zeros_left) * pow3_idx(rem - zeros_left);
            }

            if (rank > count) {
                rank -= count;
                continue;
            } else {
                result = result * 4 + d;
                if (d == 0)
                    zeros_left--;
                leading = false;
                break;
            }
        }
    }

    return result;
}
