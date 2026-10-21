/*
 * A227097  a(n) = n successive applications of the <*>(3,2) operator to 1.
 * https://oeis.org/A227097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n successive applications of the <*>(3,2) operator to 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Helpers:   apply_operator
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/apply_operator.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t value = 1;
    for (idx_t i = 1; i <= n; ++i)
        value = apply_operator(value);

    return value;
}
