/*
 * A091793  Triangle read by rows in which row n contains the smallest
 *          nontrivial set of n consecutive numbers divisible by the next n
 *          numbers respectively. The next n numbers are numbers from n(n-1)/2
 *          +1 up to n(n+1)/2.
 * https://oeis.org/A091793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A091793:  triangle read by rows.
   a(n) = first element of row r + k, where r is the least integer
   with r(r+1)/2 ≥ n and k = n - r(r-1)/2 - 1 (0‑based position in the row).
   The first element of row r is LCM{r(r-1)/2+1, …, r(r+1)/2} + r(r-1)/2 + 1.
   The values are computed directly, without memoisation, which suffices
   for n ≤ 10000 and keeps the implementation short and type‑safe. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Compute lcm of the integers in the inclusive interval [start, end]. */
static seq_t lcm_interval(idx_t start, idx_t end)
{
    seq_t cur = (seq_t)start;
    for (idx_t x = start + 1; x <= end; ++x)
    {
        seq_t g   = gcd(cur, (seq_t)x);      /* use provided seq_t gcd */
        seq_t div = cur / g;
        cur      = div * (seq_t)x;
    }
    return cur;
}

/* External linkage: sequence element a(n). */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n.  r is at most about √(2n). */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n)
        ++r;

    /* Position within the row (0‑based). */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t pos      = n - prev_tri - 1;   /* 0 ≤ pos < r */

    /* Compute the first element of row r. */
    idx_t start = r * (r - 1) / 2 + 1;
    idx_t end   = r * (r + 1) / 2;

    seq_t lcm_val = lcm_interval(start, end);
    seq_t offset  = (seq_t)r * (r - 1) / 2;
    seq_t first   = lcm_val + offset + 1;

    /* Return the desired entry in the row. */
    return first + (seq_t)pos;
}
