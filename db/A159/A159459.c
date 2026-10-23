/*
 * A159459  Rectangular array read by antidiagonals: a(n,m) = number of
 *          divisors of m that don't divide n.
 * https://oeis.org/A159459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of divisors of m that don't divide n, where (n,m)
 *            is determined by reading the 2‑D array a(i,j) by antidiagonals,
 *            starting with a(1,1)=0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A159459. */
#include <stdint.h>
#include "../utils/divisor_count_u64.h"   /* divisor_count_u64(uint64_t) -> seq_t */

/* Compute gcd of two idx_t numbers. */
static idx_t gcd_idx(const idx_t a, const idx_t b)
{
    idx_t x = a, y = b;
    while (y != 0) {
        idx_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal index t such that the n-th term lies on it.
       Number of entries up to antidiagonal t is (t-1)*t/2. */
    idx_t t = 1;
    while ((unsigned __int128)(t - 1) * t / 2 < n)
        ++t;

    /* Compute the position of n within this antidiagonal. */
    idx_t prev = (unsigned __int128)(t - 2) * (t - 1) / 2; /* entries before antidiagonal t */
    idx_t r     = n - prev;                 /* 1‑based index within the diagonal (1 .. t-1) */
    idx_t i     = t - r;                     /* row number */
    idx_t j     = r;                         /* column number */

    /* Compute d(j) and d(gcd(i,j)) using the provided divisor-count routine. */
    seq_t dj = divisor_count_u64((uint64_t)j);
    idx_t g  = gcd_idx(i, j);
    seq_t dg = divisor_count_u64((uint64_t)g);

    return dj - dg;
}
