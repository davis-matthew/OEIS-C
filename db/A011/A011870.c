/*
 * A011870  a(n) = floor(n*(n-1)/17).
 * https://oeis.org/A011870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(n-1)/17)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    /* use 128‑bit arithmetic to avoid overflow in the product */
    unsigned __int128 prod = (unsigned __int128)n * (n-1);
    /* integer division gives the floor */
    unsigned __int128 res128 = prod / 17;
    /* cast back to the sequence type, truncating if it doesn't fit */
    return (seq_t)res128;
}
