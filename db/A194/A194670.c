/*
 * A194670  Triangular array: T(n,k)=[<n*r>+<k*r>], where [ ] = floor, < > =
 *          fractional part, and r = sqrt(5).
 * https://oeis.org/A194670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor((i+j)*sqrt(5)) - floor(i*sqrt(5)) -
 *            floor(j*sqrt(5)) where i and j are the row and column indices
 *            (1‑based) of the n‑th element in the triangular array T(n,k) =
 *            ⌊{n*sqrt(5)} + {k*sqrt(5)}⌋
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for unsigned __int128.
   Returns floor(sqrt(x)). */
static inline uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t lo = 0;
    uint64_t hi = ~0ULL;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 m2 = (__int128)mid * mid;
        if (m2 <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) return 0;
            hi = mid - 1;
        }
    }
    return hi;
}

/* floor(k*sqrt(5)) computed as floor(sqrt(5*k*k)) to avoid floating point */
static inline seq_t floor_n_sqrt5(idx_t k)
{
    if (k == 0) return 0;
    unsigned __int128 prod = (unsigned __int128)5 * k * k;
    return (seq_t)isqrt_uint128(prod);
}

/* Compute the row index (1‑based) for the n‑th element in the flattened array */
static inline idx_t row_for(idx_t n)
{
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    uint64_t s = isqrt_uint128(val);
    idx_t r = (s - 1) >> 1;                  /* floor((sqrt(1+8n)-1)/2) */
    /* adjust if the triangular number is still less than n */
    if ((unsigned __int128)r * (r + 1) / 2 < n) r++;
    return r;
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, returns 0 as safe guard */

    idx_t r = row_for(n);                                            /* row i */
    idx_t prev = (unsigned __int128)r * (r - 1) / 2;                  /* elements before row r */
    idx_t c = n - prev;                                               /* column j */
    seq_t fi = floor_n_sqrt5(r);
    seq_t fj = floor_n_sqrt5(c);
    seq_t fij = floor_n_sqrt5(r + c);
    return fij - fi - fj;   /* 0 or 1 */
}
