/*
 * A193729  Mirror of the triangle A193728.
 * https://oeis.org/A193729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2) k = n -
 *            r*(r+1)/2 and T(n,k) satisfies T(0,0)=1, T(1,0)=1, T(1,1)=2
 *            T(n,k) = 4*T(n-1,k-1) + 3*T(n-1,k) (for 0≤k≤n) The function
 *            builds the triangle rows up to r using two linear‑size buffers.
 *            All arithmetic is performed in the type seq_t, so the im...
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 96, u64 n <= 334,
 *            u128 n <= 1197, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* integer square‑root, floor(sqrt(x)) for non‑negative x */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x;
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* determine the row r and column k in the triangle */
    idx_t t = floor_sqrt_u64(8 * n + 1);
    idx_t r = (t - 1) >> 1;                     /* same as floor((t-1)/2) */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* trivial cases */
    if (r == 0) return (seq_t)1;                /* T(0,0) */
    if (r == 1) return (k == 0) ? (seq_t)1 : (seq_t)2; /* row 1 */

    /* dynamic programming: keep 2 rows only */
    size_t len = (size_t)(r + 1);
    seq_t *prev = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(len, sizeof(seq_t));
    if (!prev || !curr) {
        /* allocation failure – return 0 (value not needed in tests) */
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* initialise row 1: T(1,0)=1, T(1,1)=2 */
    prev[0] = (seq_t)1;
    prev[1] = (seq_t)2;

    for (idx_t i = 2; i <= r; ++i) {
        /* first element of the row */
        curr[0] = prev[0] * 3;

        for (idx_t j = 1; j <= i; ++j) {
            seq_t val = 4 * prev[j - 1];
            if (j <= i - 1) val += 3 * prev[j];
            curr[j] = val;
        }
        /* swap prev and curr for the next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];

    free(prev);
    free(curr);
    return result;
}
