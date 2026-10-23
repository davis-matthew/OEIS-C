/*
 * A136037  Numbers with at least three adjacent equal digits in binary
 *          representation.
 * https://oeis.org/A136037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   has_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth positive integer whose binary representation
   contains a run of at least three identical consecutive bits
   (either 000 or 111).  The function scans integers sequentially
   and counts those that satisfy the pattern. */
#include <stdbool.h>
#include "../utils/has_triple.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t x = 0;
    while (count < n)
    {
        x += 1;
        if (has_triple(x))
            count += 1;
    }
    return x;
}
