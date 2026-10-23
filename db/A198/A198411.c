/*
 * A198411  a(n)= (4^(2^n) + 2^(2^n) + 1)/7.
 * https://oeis.org/A198411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^(2^n) + 2^(2^n) + 1)/7
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses 128‑bit intermediates for native unsigned
   types up to 64 bits, ensuring correct results for all n where the
   value fits in seq_t.  For larger seq_t types (e.g. arbitrary‑precision
   integers) it falls back to direct multiplication on seq_t. */

#define _GNU_SOURCE
#include <stddef.h>
#include <stdbool.h>   /* required by eval harness, not used directly */

seq_t a(idx_t n)
{
    /* Branch for native unsigned integer types up to 64 bits */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 x = 4;   /* 4^(2^0)   */
        unsigned __int128 y = 2;   /* 2^(2^0)   */

        for (idx_t i = 0; i < n; ++i) {
            x *= x;                /* x = x^2   (=4^(2^(i+1))) */
            y *= y;                /* y = y^2   (=2^(2^(i+1))) */
        }

        unsigned __int128 sum = x + y + 1;
        unsigned __int128 res  = sum / 7;   /* exact division */
        return (seq_t)res;
    }

    /* Branch for larger seq_t types (e.g., arbitrary‑precision) */
    seq_t x = 4;
    seq_t y = 2;

    for (idx_t i = 0; i < n; ++i) {
        x = x * x;
        y = y * y;
    }

    seq_t sum = x + y + (seq_t)1;
    return sum / (seq_t)7;
}
