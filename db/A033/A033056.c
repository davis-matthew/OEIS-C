/*
 * A033056  Numbers whose base-5 representation Sum_{i=0..m} d(i)*5^i has odd
 *          d(m-i) for all odd i.
 * https://oeis.org/A033056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   valid_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A033056: numbers whose base‑5 representation has odd digits at
 *  odd positions (counted from the most significant digit).   */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/valid_base5.h"

/* Return the nth term of the sequence, where n ≥ 1. */
seq_t a(idx_t n)
{
    seq_t val   = 0;   /* current candidate */
    idx_t cnt   = 0;   /* how many valid numbers found */

    while (cnt < n) {
        ++val;
        if (valid_base5(val))
            ++cnt;
    }
    return val;
}
