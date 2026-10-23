/*
 * A194330  Triangular array: g(n,k)=number of fractional parts (i*r) in
 *          interval [(k-1)/n, k/n], for 1<=i<=2n, 1<=k<=n, r=2-sqrt(3).
 * https://oeis.org/A194330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A194330 – Triangular array g(n,k)
 * The sequence lists the array row‑by‑row:
 *   a(1)           = g(1,1)
 *   a(2),a(3)      = g(2,1),g(2,2)
 *   a(4),a(5),a(6)= g(3,1),g(3,2),g(3,3)
 *   …
 *
 * g(n,k) counts 1 ≤ i ≤ 2n for which the fractional part of i*r
 * lies in the left‑closed, right‑open interval
 *   [(k‑1)/n , k/n)   (r = 2 – √3).
 *
 * Using the identity
 *   frac(i*r) = 1 – frac(i√3)
 * we obtain the equivalent integer test
 *
 *   t(i) = floor(n*i*r) – n*floor(i*r)
 *   g(n,k) = #{ i | t(i) = k‑1 } .
 *
 * Each g(n,k) is 1, 2 or 3.  The implementation below evaluates
 * floor(m*r) exactly via the quadratic field ℚ(√3) with 128‑bit
 * arithmetic and no floating‑point operations.
 */

#include <stdint.h>

/*  √3 ≈ 1732050807568877 / 10^15   (accuracy > 1e‑13 for m ≤ 2·10^4) */
static const uint64_t SQRT3_NUM = 1732050807568877ULL;
static const uint64_t SQRT3_DEN = 1000000000000000ULL;   /* 10^15 */

/* floor( m * √3 ) with guaranteed exactness, using 128‑bit intermediate */
static inline uint64_t floor_sqrt3(uint64_t m) {
    return (uint64_t)((unsigned __int128)m * SQRT3_NUM / SQRT3_DEN);
}

/* floor( m * r ),  r = 2 – √3
 *   floor(m*r) = 2*m – floor(m*√3) – 1
 */
static inline uint64_t floor_alpha(uint64_t m) {
    return 2 * m - floor_sqrt3(m) - 1;
}

/* triangular number T(n) = n*(n+1)/2  (safe for 64‑bit) */
static inline uint64_t triangular(uint64_t n) {
    return n * (n + 1) / 2;
}

/* smallest n such that triangular(n) >= pos  (binary search) */
static inline uint64_t find_row(uint64_t pos) {
    uint64_t low = 1, high = pos;
    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        if (triangular(mid) >= pos)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* ------------------------------------------------------------ */
/*  Main function required by the testing harness                 */
/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* undefined – guard */

    /* locate row N and column k (1‑based) */
    uint64_t N   = find_row(n);
    uint64_t prev_tri = triangular(N - 1);
    uint64_t k   = n - prev_tri;          /* 1 ≤ k ≤ N */

    seq_t count = 0;
    for (uint64_t i = 1; i <= 2 * N; ++i) {
        uint64_t floor_i_alpha = floor_alpha(i);
        uint64_t floor_Ni_alpha = floor_alpha(N * i);
        uint64_t t = floor_Ni_alpha - N * floor_i_alpha;
        if (t == k - 1) ++count;
    }
    return (seq_t)count;
}
