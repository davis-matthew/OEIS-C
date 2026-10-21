/*
 * A212701  Main transitions in systems of n particles with spin 3.
 * https://oeis.org/A212701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(b-1)*b^(n-1) with b = 7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n * 6 * 7^(n-1) */

#include <stddef.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) { /* offset starts at 1, but guard against n = 0 */
        return (seq_t)0;
    }

    /* Compute 7^(n-1) using the arbitrary‐precision pow routine */
    seq_t pow7 = seq_pow((seq_t)7, n - 1);

    /* Multiply by 6 and then by n, each cast to seq_t preserves type */
    seq_t tmp = pow7 * (seq_t)6;
    seq_t result = tmp * (seq_t)n;

    return result;
}
