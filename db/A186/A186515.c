/*
 * A186515  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=4+5j^2. Complement of
 *          A186516.
 * https://oeis.org/A186515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt((n^2)/5 - 7/10))
 * Range:     u8 n <= 12, u16 n <= 71, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A186515 using only integer arithmetic. */
#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* Compute floor((2*n*n - 7)/10) using signed arithmetic to avoid
       underflow when n = 1.  The result fits easily into a signed long long
       for all intended use (n up to thousands). */
    long long nll = (long long)n;
    long long val = (2LL * nll * nll - 7) / 10;
    if (val < 0) val = 0;                       /* ensure non‑negative */

    seq_t sqrt_part = isqrt_seq((seq_t)val);     /* floor(sqrt(val))   */
    return (seq_t)n + sqrt_part;
}
