/*
 * A236858  Irregular table where row n contains numbers from 1 to the least
 *          common multiple (LCM) of {1, 2, ..., n}. Row 0 is given as a(0)=1.
 * https://oeis.org/A236858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 395, u16 n <= 6441, u32 n <= 6441, u64 n <= 6441,
 *            u128 n <= 6441, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the flattening of an irregular table whose row r contains
 * the numbers 1 … LCM(1,…,r).
 * For a given index n we find the first row m for which the cumulative
 * number of entries up to that row exceeds n.  The value a(n) is the
 * offset of n within that row plus 1.
 *
 * Implementation: iterate over rows, keeping lcm(1..r) incrementally
 * and the cumulative sum of entries.  As soon as the sum surpasses
 * n, compute the offset.
 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t r = 0;          // current row index
    seq_t lcm = 1;        // lcm(1..r)
    seq_t sum = 0;        // cumulative number of entries up to row r

    for (;;) {
        // add current row to cumulative count
        sum += lcm;

        // if n lies within this row, compute the answer
        if ((seq_t)n < sum) {
            seq_t prev = sum - lcm;            // entries before this row
            seq_t offset = (seq_t)n - prev;    // 0‑based position in row
            return offset + 1;                 // a(n) = position + 1
        }

        /* move to next row: r++ and update lcm */
        ++r;
        seq_t g = gcd(lcm, (seq_t)r);
        lcm = (lcm / g) * r;      // lcm(1..r) = lcm(previous) * r / gcd(...)
    }
}
