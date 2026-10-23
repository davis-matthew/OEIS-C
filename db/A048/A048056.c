/*
 * A048056  Duplicate of A038534.
 * https://oeis.org/A048056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (C(2n,n) / 2^{popcount(n)})^2 where C denotes the
 *            binomial coefficient and popcount(n) is the number of 1‑bits in
 *            the binary representation of n
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 10, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* count number of 1 bits in a 64‑bit unsigned integer */
static inline size_t popcount_u64(idx_t n)
{
    size_t c = 0;
    while (n)
    {
        n &= n - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* compute binomial coefficient binom = C(2n, n) */
    seq_t binom = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        binom = binom * (seq_t)(n + k) / (seq_t)k;
    }

    /* divide by 2^{popcount(n)} */
    size_t pc = popcount_u64(n);
    seq_t pow2 = 1;
    for (size_t i = 0; i < pc; ++i)
        pow2 *= 2;
    binom = binom / pow2;

    /* return the square */
    return binom * binom;
}
