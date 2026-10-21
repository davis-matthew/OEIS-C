/*
 * A202095  Number of (n+2)X5 binary arrays avoiding patterns 001 and 011 in
 *          rows and columns.
 * https://oeis.org/A202095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4)^4*(n+6)^4*(n+8)^2/147456 for n even, a(n) =
 *            (n+3)^2*(n+5)^4*(n+7)^3*(n+9)/147456 for n odd
 * Range:     u8 none, u16 n <= 4, u32 n <= 24, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   compute_using_128, compute_using_bigint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_using_128.h"
#include "../utils/compute_using_bigint.h"

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate only when seq_t is small (≤64 bits). */
    if (sizeof(seq_t) <= 8)
        return compute_using_128(n);
    else
        return compute_using_bigint(n);
}
