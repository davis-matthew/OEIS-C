/*
 * A193635  Triangle: T(n,k)=C(3n-k,k), 0<=k<=n.
 * https://oeis.org/A193635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle:  T(n,k) = C(3n-k , k)
   a(n) is the n-th element in the row‑wise enumeration of the
   triangle starting with T(0,0)=1, T(1,0)=1, T(1,1)=2, T(2,0)=1, …   */

#include <stdint.h>

/* ------------------------------------------------------------ */
/* Integer square‑root: floor(sqrt(x)) for 64‑bit unsigned x    */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    /* largest power of four ≤ 2^63 */
    idx_t bit = 1ULL << 62;
    while (bit > x) bit >>= 2;          /* find the highest bit */
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ------------------------------------------------------------ */
/* Binomial coefficient C(n,k) computed multiplicatively        */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;           /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 tmp = (unsigned __int128)result
                               * (unsigned __int128)(n - k + i);
        tmp /= (unsigned __int128)i;    /* exact division */
        result = (seq_t)tmp;
    }
    return result;
}

/* ------------------------------------------------------------ */
/* Main sequence generator                                         */
seq_t a(idx_t n)
{
    /* Determine the row r and column k of the triangle entry     */
    idx_t val = 8 * n + 1;
    idx_t s   = isqrt_u64(val);
    idx_t r   = (s - 1) / 2;                   /* row index           */
    idx_t prev = r * (r + 1) / 2;              /* cumulative count up to previous row */
    idx_t k   = n - prev;                      /* column index        */

    /* Compute the binomial coefficient T(r,k) = C(3r-k , k)     */
    idx_t top = 3 * r - k;                     /* numerator in binom  */
    return binom_idx(top, k);
}
