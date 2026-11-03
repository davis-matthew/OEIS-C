/*
 * A183653  Number of (n+1) X (n+1) 0..5 arrays with every 2 X 2 subblock
 *          summing to 10.
 * https://oeis.org/A183653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*12^n + 20*10^n - 18*9^n - 32*8^n + 24*6^n - 20*5^n +
 *            8*4^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 807 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

/* Return the n-th term (n >= 1) of OEIS sequence A183653. */
seq_t a(idx_t n)
{
    seq_t pow4  = ipow_u64_idx(4 , n);
    seq_t pow5  = ipow_u64_idx(5 , n);
    seq_t pow6  = ipow_u64_idx(6 , n);
    seq_t pow8  = ipow_u64_idx(8 , n);
    seq_t pow9  = ipow_u64_idx(9 , n);
    seq_t pow10 = ipow_u64_idx(10, n);
    seq_t pow12 = ipow_u64_idx(12, n);

    seq_t result = 24*pow12  /* 24*12^n */
                 + 20*pow10  /* +20*10^n */
                 - 18*pow9   /* -18*9^n */
                 - 32*pow8   /* -32*8^n */
                 + 24*pow6   /* +24*6^n */
                 - 20*pow5   /* -20*5^n */
                 + 8*pow4;   /* +8*4^n */

    return result;
}
