/*
 * A172165  A simple sequence a(n) = n + n^(n-1).
 * https://oeis.org/A172165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + n^(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* Undefined for n=0; defensive return */
    seq_t pow = seq_pow((seq_t)n, n - 1);   /* Compute n^(n-1) */
    return pow + (seq_t)n;                    /* a(n) = n + n^(n-1) */
}
