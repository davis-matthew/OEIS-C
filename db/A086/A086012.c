/*
 * A086012  Number of 4's in decimal expansion of n^2.
 * https://oeis.org/A086012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of times the digit ‘4’ occurs in the decimal
 *            representation of n²
 * Range:     u8 n <= 17, u16 n <= 410, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 984 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the digit '4' in the decimal expansion of a number stored in seq_t. */
static inline seq_t count_fours(seq_t v)
{
    seq_t cnt = 0;
    if (v == 0) return 0;            /* 0 has no digit 4 */
    while (v > 0) {
        seq_t d = v % 10;
        if (d == 4) ++cnt;
        v /= 10;
    }
    return cnt;
}

/* OEIS A086012: number of 4's in decimal expansion of n². */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t sq = nn * nn;           /* compute n² in seq_t */
    return count_fours(sq);       /* count digit 4s in the result */
}
