/*
 * A194318  Triangular array: g(n,k)=number of fractional parts (i*sqrt(8)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=2n, 1<=k<=n.
 * https://oeis.org/A194318
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

/* a(n) of OEIS A194318.
   The sequence is defined by the triangular array
   G(m,k) = # { i : 1 ≤ i ≤ 2m , (k-1)/m ≤ {i·√8} < k/m }.
   The terms a(1),a(2),… are obtained by traversing the array row by row
   (row m has m columns).  This implementation computes the required row
   and column for a given n, then counts the i’s that fall into the
   corresponding interval using only integer arithmetic.  The routine
   works for any unsigned integer type seq_t, including arbitrary‑precision
   types, and is O(√n) per call. */
#include <stdint.h>

/* Integer square root for 128‑bit numbers, used to evaluate
   floor(i·√8) as floor(√(8·i²)).  The routine is generic and works
   for any seq_t that can hold 128‑bit values.                                      */
static inline uint64_t floor_sqrt_u128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFFFFFFFFFULL;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;  /* floor(sqrt(x)) */
}

/* Compute a(n).  n is 1‑based; a(1) = 2.  For n==0 the function returns
   0, which is harmless because the offset of the sequence is 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* ---- 1. Find the row m such that m(m+1)/2 ≥ n. ---- */
    uint64_t m = 0;
    while ((unsigned __int128)m * (m + 1) / 2 < n) m++;

    /* ---- 2. Determine the column in this row (1‑based). ---- */
    uint64_t prev = (unsigned __int128)(m - 1) * m / 2;
    uint64_t kcol = n - prev;   /* 1 ≤ kcol ≤ m */

    /* ---- 3. Count the i’s that lie in the required interval. ---- */
    uint64_t count = 0;
    for (uint64_t i = 1; i <= 2 * m; ++i) {
        /* a_i = floor(i·√8) = floor(√(8·i²)) */
        uint64_t ai = floor_sqrt_u128((unsigned __int128)8 * i * i);

        /* bi = floor( (m·i)·√8 ) = floor(√(8·(m·i)²)) */
        uint64_t mj = m * i;
        uint64_t bi = floor_sqrt_u128((unsigned __int128)8 * mj * mj);

        /* k_i = floor(m·{i·√8}) + 1 = bi – m·a_i + 1    */
        uint64_t ki = bi - (m * ai) + 1;
        if (ki == kcol) ++count;
    }

    return (seq_t)count;
}
