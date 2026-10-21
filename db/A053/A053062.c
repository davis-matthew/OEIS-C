/*
 * A053062  Concatenate n, 2n, 3n, ... nn.
 * https://oeis.org/A053062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn, less
 * Formula:   a(n) = concat(n, 2n, 3n, …, n*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, but guard for safety */

    seq_t result = 0;
    seq_t n_seq = (seq_t)n;          /* sequence representation of n */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t mult = n_seq * i;                 /* i * n */
        int d = digits((idx_t)mult);            /* decimal digits of mult */
        result = result * pow10((idx_t)d) + mult;
    }
    return result;
}
