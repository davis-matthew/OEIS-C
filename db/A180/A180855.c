/*
 * A180855  Square array read by antidiagonals: T(m,n) is the Wiener index of
 *          the banana tree B(n,k) (n>=1, k>=2). B(n,k) is the graph obtained
 *          by taking n copies of a star graph on k nodes and connecting with
 *          an edge one leaf of each of these n stars with an additional node.
 * https://oeis.org/A180855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Wiener index of the banana tree B(m,k) read by
 *            antidiagonals. T(m,k) = m(k-1)(3mk-2k+2). For index n ≥ 1 the
 *            pair (m,k) is determined by the antidiagonal enumeration: let d
 *            = ceil((sqrt(8n+1)-1)/2), p = n - (d-1)d/2, m = d - p + 1, k = p
 *            + 1. Then a(n) = T(m,k)
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ---------- Integer square root (floor) for uint64_t ---------- */
static inline idx_t _floor_isqrt_u64(idx_t x)
{
    /* Uses binary search; works for all x < 2^64. */
    idx_t lo = 0;
    idx_t hi = x;
    while (hi - lo > 1) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid <= x / mid) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    return lo;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* 1. Find diagonal d such that d(d+1)/2 ≥ n. */
    idx_t t = 8 * n + 1;                      /* 8n+1 */
    idx_t sqrt_t = _floor_isqrt_u64(t);       /* floor(sqrt(8n+1)) */
    if (sqrt_t * sqrt_t < t)                 /* if not a perfect square */
        sqrt_t++;                             /* make it ceil(sqrt) */

    idx_t root_floor = (sqrt_t - 1) / 2;      /* floor of (-1+sqrt)/2 */
    idx_t d = root_floor;                     /* candidate diagonal */

    /* ensure d is the smallest satisfying d(d+1)/2 ≥ n */
    while ( ((d * (d + 1)) / 2) < n )
        ++d;

    /* 2. Determine position p within the diagonal. */
    idx_t prev_total = ((d - 1) * d) / 2;      /* sum up to previous diagonal */
    idx_t p = n - prev_total;                 /* 1 ≤ p ≤ d */

    /* 3. Retrieve m and k for T(m,k). */
    idx_t m = d - p + 1;      /* first argument of T */
    idx_t k = p + 1;          /* second argument of T */

    /* 4. Compute T(m,k) = m*(k-1)*(3*m*k - 2*k + 2). */
    seq_t seq_m   = (seq_t)m;
    seq_t seq_k   = (seq_t)k;
    seq_t k_minus = seq_k - 1;

    seq_t inner   = 3 * seq_m * seq_k - 2 * seq_k + 2u;
    seq_t result  = seq_m * k_minus * inner;

    return result;
}
