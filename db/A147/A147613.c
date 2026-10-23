/*
 * A147613  Numbers that are not Jacobsthal numbers.
 * https://oeis.org/A147613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   is_jacobsthal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A147613 – numbers that are not Jacobsthal numbers.
   Computation: iterate integers and skip those x satisfying 3*x±1 is a power of 2. */

#include <stdbool.h>
#include "../utils/is_jacobsthal.h"

/* a(n) returns the n-th number not appearing in A001045 (Jacobsthal numbers).
   The sequence is 1‑indexed. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; n=0 is undefined but just return 0 */
        return (seq_t)0;

    idx_t count = 0;     /* number of non‑Jacobsthal numbers found so far */
    seq_t cur = 0;       /* current integer being tested */

    while (count < n) {
        cur = cur + 1;           /* advance to next integer */
        if (!is_jacobsthal(cur))
            count++;             /* count it if it's not a Jacobsthal number */
    }

    return cur;
}
