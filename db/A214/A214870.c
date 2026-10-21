/*
 * A214870  Natural numbers placed in table T(n,k) layer by layer. The order
 *          of placement: at the beginning filled odd places of layer
 *          clockwise, next - even places counterclockwise. T(n,k) read by
 *          antidiagonals.
 * https://oeis.org/A214870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Implements the OEIS A214870 table‑to‑sequence conversion. The
 *            sequence is defined by a layered table T(n,k). For a given index
 *            n the antidiagonal coordinates (i,j) are found and then the
 *            table value T(i,j) is evaluated by the formulas in the OEIS
 *            entry. All arithmetic is performed in the seq_t ty...
 * Range:     u8 n <= 136, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   _T_table, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"   /* provides isqrt_u64() */
#include "../utils/_T_table.h"

seq_t a(idx_t n)
{
    /* n >= 1 as per OEIS offset */
    /* Compute the antidiagonal layer number t = floor((sqrt(8*n-7)-1)/2) */
    idx_t s = isqrt_u64(8 * n - 7);
    idx_t t = (s - 1) / 2;

    /* Compute coordinates (i,j) in the table */
    idx_t i = n - t * (t + 1) / 2;
    idx_t j = (t * t + 3 * t + 4) / 2 - n;

    return _T_table(i, j);
}
