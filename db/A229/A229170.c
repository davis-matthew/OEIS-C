/*
 * A229170  Define a sequence of real numbers by b(1)=2, b(n+1) = b(n) +
 *          log_2(b(n)); a(n) = ceiling( b(n) ).
 * https://oeis.org/A229170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(b(n)), where b(1)=2, b(n+1)=b(n)+log_2(b(n))
 * Range:     u8 n <= 43, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is defined only for n ≥ 1 */
        return 0;

    long double b = 2.0L;                /* b(1) */
    for (idx_t i = 1; i < n; ++i)        /* compute b(2)…b(n) */
        b += log2l(b);

    /* use a tiny epsilon to guard against rounding errors that could
       push a non‑integer just below an integer and make ceill wrong */
    const long double eps = 1e-12L;
    uint64_t tmp = (uint64_t)ceill(b - eps);   /* result fits in 64 bits for n ≤ 10000 */

    return (seq_t)tmp;                       /* cast safely to seq_t (any width) */
}
