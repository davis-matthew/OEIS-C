/*
 * A056822  Nearest integer to n^2/16.
 * https://oeis.org/A056822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n^2 / 16 a(n) = floor( (n*n + 8) / 16
 *            )
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 val = (nn * nn + 8) / 16;
    return (seq_t)val;
}
