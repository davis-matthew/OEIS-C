/*
 * A159841  Triangle T(n,k) = binomial(3*n+1, 2*n+k+1), read by rows.
 * https://oeis.org/A159841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 90, u64 n <= 324,
 *            u128 n <= 1175, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A159841:
   The flattened triangle T(r,c) = binomial(3*r+1, 2*r+c+1) read row‑wise.
   For an index n≥0:
       r = floor((sqrt(8*n+1)-1)/2)
       c = n - r*(r+1)/2
   then a(n) = T(r,c). */

#include <stdint.h>   /* for uint64_t */

static inline idx_t _row_of(idx_t n)
{
    /* binary search for largest r with r*(r+1)/2 ≤ n */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __int128 lhs = (__int128)mid * (mid + 1) / 2;
        if (lhs <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

static inline seq_t _binom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;      /* symmetry */
    seq_t res = 1;                 /* start with 1 */
    for (idx_t i = 1; i <= K; ++i) {
        res = res * (seq_t)(N - K + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t r = _row_of(n);
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;
    idx_t N = 3 * r + 1;
    idx_t K = 2 * r + c + 1;
    return _binom(N, K);
}
