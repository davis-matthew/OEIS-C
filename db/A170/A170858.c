/*
 * A170858  a(0) = 1, a(1) = 0; a(2^i + j) = 3a(j) + a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 0,
   a(2^i + j) = 3*a(j) + a(j + 1)  for 0 <= j < 2^i  (i >= 0) */

#include <stdint.h>

/* compute floor(log2(v)) for v > 0, works for uint64_t */
static inline idx_t floor_log2_uint64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* main routine */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* recursive step */
    idx_t i   = floor_log2_uint64(n);          /* highest power of 2 <= n  */
    idx_t pow = ((idx_t)1) << i;               /* 2^i                       */
    idx_t j   = n - pow;                       /* j = n - 2^i  (0 <= j < 2^i) */

    seq_t v0 = a(j);         /* a(j)                      */
    seq_t v1 = a(j + 1);     /* a(j + 1)                  */

    return v0 * (seq_t)3 + v1;
}
