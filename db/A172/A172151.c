/*
 * A172151  Number of partitions of n into two nonsquares.
 * https://oeis.org/A172151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ i + j = n | i > 0, j > 0, i,j are not perfect squares
 *            }
 * Range:     u8 n <= 556, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of unordered partitions of n into two non‑squares. */

#include <math.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* integer square root: floor(sqrt(n)) for idx_t values */
static inline idx_t _int_sqrt(idx_t n)
{
    /* double sqrt is accurate for n <= 10⁹; for larger values the
       subsequent corrections keep the result exact. */
    idx_t r = (idx_t)sqrt((double)n);
    while ((r + 1) * (r + 1) <= n)
        ++r;
    while (r * r > n)
        --r;
    return r;
}

/* test whether an idx_t value is a perfect square */
static inline bool _is_square(idx_t v)
{
    idx_t r = _int_sqrt(v);
    return r * r == v;
}

/* Main function */
seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t i = 1; i <= n / 2; ++i) {
        if (!_is_square(i) && !_is_square(n - i))
            ++cnt;
    }
    return cnt;
}
