/*
 * A232637  Odious numbers of order 2: a(n) = A000069(A000069(n)).
 * https://oeis.org/A232637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A000069(A000069(n))
 * Range:     u8 n <= 65, u16 n <= 8193, u32 n <= 8193, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Helpers:   nth_odious, odious_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/nth_odious.h"
#include "../utils/odious_parity.h"

/* External function required by the build. */
seq_t a(idx_t n)
{
    seq_t inter = nth_odious(n);          /* A000069(n)          */
    return nth_odious((idx_t)inter);      /* A000069(A000069(n)) */
}
