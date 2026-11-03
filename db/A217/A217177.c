/*
 * A217177  a(n) is the number of digits in the decimal representation of the
 *          smallest power of n that contains two consecutive identical
 *          digits.
 * https://oeis.org/A217177
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 103 terms through n = 9769 (2026-11-01)
 * Helpers:   ipow_u64_idx, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of digits of the smallest power of n that contains two consecutive identical digits. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/num_digits.h"

/* Test whether a seq_t value has any pair of consecutive equal decimal digits. */
static inline bool no_consecutive_equal_digits(seq_t val) {
    if (val == (seq_t)0) return true;          /* 0 contains only one digit */
    int prev = -1;                             /* previous digit, -1 means none yet */
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);        /* current least‑significant digit */
        if (prev == d) return false;           /* found equal neighbours */
        prev = d;
        tmp /= (seq_t)10;                      /* shift right one decimal digit */
    }
    return true;                               /* all adjacent digits differ */
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* offset starts at 2 */

    idx_t k = 1;
    while (1) {
        seq_t cur = ipow_u64_idx((uint64_t)n, k);
        if (!no_consecutive_equal_digits(cur)) {
            return (seq_t)num_digits(cur);
        }
        ++k;
    }
}
