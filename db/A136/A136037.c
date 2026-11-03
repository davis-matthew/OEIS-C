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
 * Validated: standard, pass, 82 terms through n = 1973 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth positive integer whose binary representation
   contains a run of at least three identical consecutive bits
   (either 000 or 111).  The function scans integers sequentially
   and counts those that satisfy the pattern. */
#include <stdbool.h>

static bool has_triple(seq_t v)
{
    /* Check every group of three consecutive bits. */
    while (v != 0)
    {
        seq_t last3 = v & 7;                 /* mask the lowest three bits */
        if (last3 == 0 || last3 == 7)        /* 000 or 111 */
            return true;
        v >>= 1;                             /* slide window by one bit */
    }
    return false;
}

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
