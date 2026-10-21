/*
 * A236347  Manhattan distances between n and 2*n in a left-aligned triangle
 *          with next M natural numbers in row M: 1, 2 3, 4 5 6, 7 8 9 10,
 *          etc.
 * https://oeis.org/A236347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |row(n)-row(2n)| + |col(n)-col(2n)|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 128‑bit integer type (compiler that supports it) */
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 U128;
#else
#error "Compiler must support 128‑bit unsigned integers."
#endif

/* triangular number before row r: T_{r-1} = (r-1)*r/2 */
static inline idx_t tri_before(idx_t r)
{
    return (idx_t)((U128)(r - 1) * r / 2);
}

/* smallest r such that r*(r+1)/2 >= n,  n>=1 */
static inline idx_t row_of(idx_t n)
{
    idx_t low = 1, high = 1;

    /* Find an upper bound by exponential search */
    while ((U128)high * (high + 1) / 2 < n)
        high <<= 1;

    /* Binary search within [low, high] */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((U128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* column of number k in its row (1‑based). */
static inline idx_t col_of(idx_t k)
{
    idx_t r = row_of(k);
    return k - tri_before(r);
}

/* sequence value a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for offset 1, but safe guard */

    /* positions of n and 2*n */
    idx_t r1 = row_of(n);
    idx_t c1 = col_of(n);

    idx_t n2 = n << 1;          /* 2*n fits in idx_t for all valid n */
    idx_t r2 = row_of(n2);
    idx_t c2 = col_of(n2);

    /* Manhattan distance */
    idx_t dr = r1 > r2 ? r1 - r2 : r2 - r1;
    idx_t dc = c1 > c2 ? c1 - c2 : c2 - c1;

    return (seq_t)(dr + dc);
}
