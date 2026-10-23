/*
 * A170776  a(n) = n^8*(n^4 + 1)/2.
 * https://oeis.org/A170776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 * (n^4 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 42, u128 n <= 1721,
 *            big any n
 * Helpers:   a_big, a_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a_big.h"
#include "../utils/a_small.h"

seq_t a(idx_t n)
{
    /* The same code path works for all seq_t sizes: a_small is used
       when the underlying integer type is ≤64 bits; a_big is used
       otherwise (e.g. 128‑bit or arbitrary‑precision). */
    if (sizeof(seq_t) <= 8)
        return a_small(n);
    else
        return a_big(n);
}
