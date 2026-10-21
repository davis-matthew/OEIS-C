/*
 * A075670  Sum of next n 9th powers.
 * https://oeis.org/A075670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=n(n-1)/2+1 .. n(n-1)/2+n} i^9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 14, u128 n <= 148,
 *            big any n
 * Helpers:   pow9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A075670 by summing the next n ninth powers. */

#include <stdint.h>
#include "../utils/pow9.h"

seq_t a(idx_t n)
{
    /* cast the index to seq_t first */
    seq_t nn = (seq_t)n;

    /* starting index of the block of n consecutive 9th powers */
    seq_t start = nn * (nn - (seq_t)1) / (seq_t)2 + (seq_t)1;

    seq_t sum = (seq_t)0;
    for (idx_t j = 0; j < n; ++j)
    {
        seq_t val = start + (seq_t)j;
        sum += pow9(val);
    }
    return sum;
}
