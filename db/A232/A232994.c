/*
 * A232994  a(n) = 6*(n - 3)*(n - 4)*2^(n-3)*n^(n-4).
 * https://oeis.org/A232994
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*(n-3)*(n-4)*2^(n-3)*n^(n-4) (for n >= 3)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 3.  For n = 3 or 4 the
       factor (n-4) is zero, so the entire term is zero.  For n < 5
       the subtraction would underflow a uint64_t, so guard explicitly. */
    if (n < 5) {
        return (seq_t)0;
    }

    /* Compute the integer part 6*(n-3)*(n-4) exactly in seq_t. */
    seq_t part = (seq_t)6;
    part *= (seq_t)(n - 3);
    part *= (seq_t)(n - 4);

    /* 2^(n-3) and n^(n-4) are computed with big_pow to avoid
       any shift‑by‑large‑amount UB that a simple << might cause. */
    seq_t pow2  = big_pow((seq_t)2, (seq_t)(n - 3));
    seq_t pown  = big_pow((seq_t)n, (seq_t)(n - 4));

    /* Final value: product of the three factors. */
    return part * pow2 * pown;
}
