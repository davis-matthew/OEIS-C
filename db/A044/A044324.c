/*
 * A044324  Numbers n such that string 8,1 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑9 representation
 *            contains the consecutive digits "81" while the preceding integer
 *            does not. Computed by linear search, counting candidates until
 *            the nth term is found
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has81_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has81_idx.h"

/* OEIS A044324 : 1‑based index */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    while (count < n)
    {
        if (has81_idx(k) && !has81_idx(k - 1))
        {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        k++;
    }
    /* Unreachable; returned to satisfy compiler. */
    return (seq_t)0;
}
