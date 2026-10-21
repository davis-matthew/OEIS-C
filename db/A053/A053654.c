/*
 * A053654  Multiples of 123456789.
 * https://oeis.org/A053654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 123456789 * n
 * Range:     u8 none, u16 none, u32 n <= 34, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A053654 consists of multiples of 123456789. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to seq_t and multiply. This works for all
       supported seq_t types (8,16,32,64-bit, 128-bit or arbitrary-precision). */
    return (seq_t)n * (seq_t)123456789;
}
