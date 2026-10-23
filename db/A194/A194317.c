/*
 * A194317  Triangular array: g(n,k)=number of fractional parts (i*sqrt(8)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n, 1<=k<=n.
 * https://oeis.org/A194317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Computes A194317 by evaluating the triangular array entry g(r,k)
 *            defined in the OEIS entry. For a linear index n, the function
 *            finds the corresponding row r and column k, then counts how many
 *            integers i (1≤i≤r) satisfy floor( sqrt(8*(i*r)²) ) - r*floor(
 *            sqrt(8*i²) ) == k-1. This uses only integer...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t floor_sqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = 1ULL << 32;
    uint64_t ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Find row r such that T_{r-1} < n <= T_r, T_r = r(r+1)/2 */
    uint64_t m = (uint64_t)n;
    uint64_t t = 1 + 8 * m;
    uint64_t s = floor_sqrt_uint64(t);      /* floor(sqrt(1+8m)) */
    uint64_t r = (s - 1) >> 1;              /* floor((s-1)/2) */
    if ((unsigned __int128)s * s < t) r++;  /* ceil correction */

    uint64_t prev = (r + 1ULL) * r / 2;     /* T_{r} = r(r+1)/2 */
    prev = (prev - r);                     /* T_{r-1} = r(r-1)/2 */
    uint64_t k = m - prev;                 /* column (1‑based) */
    uint64_t target = k - 1;                /* diff comparison value */

    uint64_t count = 0;
    for (uint64_t i = 1; i <= r; ++i) {
        uint64_t ai   = floor_sqrt_uint64(8 * i * i);          /* floor(i√8)   */
        uint64_t prod = i * r;
        uint64_t ar   = floor_sqrt_uint64(8 * prod * prod);  /* floor((ir)√8) */
        uint64_t diff = ar - r * ai;
        if (diff == target) ++count;
    }

    return (seq_t)count;
}
