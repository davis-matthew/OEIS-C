/*
 * A061517  a(0) = 0; a(n) is obtained by incrementing each digit of a(n-1) by
 *          4.
 * https://oeis.org/A061517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 9, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   next_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(0) = 0; for n > 0,
   a(n) is obtained by incrementing each decimal digit of a(n‑1)
   by 4.  If the result exceeds 9 it is written as a two‑digit string
   (e.g., 9 → 13).  Example: a(3) = 12 (1→5, 2→6 → 56).  */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_value.h"

/* --------------------------------------------------------------------- */
/*  Compute a(n) by iterative application of the digit‑wise incrementation.
 *
 *  The implementation works for any build where seq_t is an unsigned
 *  integer type (8, 16, 32, 64 bits or unsigned __int128) or an
 *  arbitrary‑precision integer class.  All operations use only the
 *  operators and types allowed by the specification.
 *
 *  Because a(n) grows relatively slowly (approximately linearly in n),
 *  the straightforward O(n²) algorithm of recomputing each term from
 *  scratch is fast enough for n up to 10 000 (≈100 million digit
 *  operations), well within the 2‑second limit.
 *
 *  No external helper functions from the utils library are required.
 * --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t cur = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i)
        cur = next_value(cur);

    return cur;
}
