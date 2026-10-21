/*
 * A063037  Numbers without 3 consecutive equal binary digits.
 * https://oeis.org/A063037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th integer (starting from 0) whose binary
 *            representation * contains no three consecutive equal bits. This
 *            is computed by a simple * linear scan checking each candidate
 * Range:     u8 n <= 88, u16 n <= 4180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_no_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: uses only integer operators and bool; works for all seq_t types. */

#include <stdbool.h>
#include "../utils/has_no_three.h"

seq_t a(idx_t n)
{
    /* We count from 0 upwards until we have found the n-th valid number. */
    idx_t  found = 0;
    seq_t  x     = 0;

    while (1) {
        if (has_no_three(x)) {
            found++;
            if (found == n) return x;
        }
        x++;
    }
}
