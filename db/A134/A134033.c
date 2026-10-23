/*
 * A134033  Rectangular array T(n,k) read by antidiagonals. Row n consists of
 *          numbers k for which {k*sqrt(2)}<n/k, where { } denotes fractional
 *          part.
 * https://oeis.org/A134033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is computed by reading the infinite array * T defined by:
 *            for each row i the j-th element is the * j-th smallest integer k
 *            satisfying {k*√2} < i/k. * The array is read by antidiagonals:
 *            for each sum s = i+j * we list (i,j) with increasing i
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   get_sqrt2_scaled
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic.   */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_sqrt2_scaled.h"

/* Test whether {k*√2} < i/k  using fixed‑point arithmetic. */
static bool cond(idx_t k, idx_t i)
{
    const unsigned __int128 SCALE = get_sqrt2_scaled();          /* 2^63 part of √2 */
    const unsigned __int128 MASK  = (((unsigned __int128)1 << 63) - 1);
    const unsigned __int128 SHIFT = ((unsigned __int128)1 << 63); /* 2^63 */

    unsigned __int128 product   = (unsigned __int128)k * SCALE;   /* k * √2 * 2^63  */
    unsigned __int128 fraction  = product & MASK;                /* lower 63 bits */

    unsigned __int128 left  = fraction * k;
    unsigned __int128 right = (unsigned __int128)i * SHIFT;
    return left < right;
}

/* Return the j‑th element (i.e., the j‑th integer k) of row i. */
static seq_t get_k(idx_t i, idx_t j)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (cond(k, i)) {
            ++count;
            if (count == j)
                return (seq_t)k;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find diagonal sum S such that (S-1)*S/2 >= n. */
    idx_t lo = 2, hi = 2;
    while (((__uint128_t)(hi - 1) * hi) / 2 < n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t val = ((__uint128_t)(mid - 1) * mid) / 2;
        if (val >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t S = lo;

    /* Number of items before this diagonal */
    __uint128_t prev = ((__uint128_t)(S - 2) * (S - 1)) / 2;
    idx_t offset = (idx_t)(n - prev);          /* 1‑based position on diagonal */
    idx_t i = offset;                          /* row index (1‑based) */
    idx_t j = S - offset;                      /* column index (1‑based) */

    return get_k(i, j);
}
