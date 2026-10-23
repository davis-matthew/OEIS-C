/*
 * A195437  Triangle formed by: 1 even, 2 odd, 3 even, 4 odd... starting with
 *          2.
 * https://oeis.org/A195437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*n + 2 + floor((sqrt(1+8*n)-1)/2)
 * Range:     u8 n <= 119, u16 n <= 5049, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* count of triangular numbers <= n */
    long double v = (long double)1.0 + (long double)8.0 * (long double)n;
    idx_t k = (idx_t)((sqrt(v) - 1.0) / 2.0);

    seq_t n_seq = (seq_t)n;
    seq_t result = 2 * n_seq + 2 + (seq_t)k;
    return result;
}
