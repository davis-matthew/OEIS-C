/*
 * A188588  Row sums of 1-Euler triangle A188587.
 * https://oeis.org/A188588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*a(n-1) for n>=3; a(0)=1, a(1)=2, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   compute_direct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = (n+1)!/2 for n>=2 */

#include <stdlib.h>
#include "../utils/compute_direct.h"

seq_t a(idx_t n)
{
    return compute_direct(n);
}
