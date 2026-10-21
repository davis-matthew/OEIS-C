/*
 * A218219  Define a(x,y) to be 1 if x is 0 or 1 and y*a(x-1,y)-a(x-2,y)
 *          otherwise. Then the n-th term of the sequence is a(n,n).
 * https://oeis.org/A218219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n,n) where a(x,y) = 1 if x ≤ 1 = y * a(x‑1,y) –
 *            a(x‑2,y) otherwise (OEIS A218219)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   compute_column
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for size_t */
#include "../utils/compute_column.h"

seq_t a(idx_t n)
{
    return compute_column(n);
}
