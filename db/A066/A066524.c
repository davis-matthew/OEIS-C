/*
 * A066524  a(n) = n*(2^n - 1).
 * https://oeis.org/A066524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2^n - 1) (offset 0)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes A066524: a(n) = n*(2^n - 1) for n >= 0.
   Works for seq_t being any unsigned integer type (uint8_t …, unsigned __int128)
   or an arbitrary‑precision integer class supporting shift and multiplication. */
seq_t a(idx_t n)
{
    /* Compute 2^n as a power of two by repeated left shift. */
    seq_t pow = 1;                 /* 2^0 */
    idx_t i = n;
    while (i) {                    /* loop n times, shifting left by 1 each time */
        pow <<= 1;
        --i;
    }

    /* Result = n * (2^n - 1).  Cast n to seq_t for the multiplication. */
    return ((seq_t)n) * (pow - (seq_t)1);
}
