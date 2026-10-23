/*
 * A049781  a(n) = (b(n) + 1)/2, where b = A000959 (the lucky numbers).
 * https://oeis.org/A049781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (b(n) + 1)/2, where b(n) is the nth lucky number
 *            (A000959)
 * Range:     u8 n <= 48, u16 n <= 6158, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _nth_lucky
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_nth_lucky.h"

/* External entry point for OEIS A049781. */
seq_t a(idx_t n)
{
    return _nth_lucky(n);
}
