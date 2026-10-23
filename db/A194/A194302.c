/*
 * A194302  Triangular array: g(n,k)=number of fractional parts (i*sqrt(5)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=2n, 1<=k<=n.
 * https://oeis.org/A194302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = g(r,k) where r = ceil((sqrt(8n+1)-1)/2) and k = n -
 *            r(r-1)/2 g(r,k) = #{i : 1 ≤ i ≤ 2r and floor(r*i*√5) –
 *            r*floor(i*√5) = k-1 }
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the count of fractional parts of i*√5 that lie in the
   interval [(k-1)/r , k/r] (k=1..r), without using floating‑point arithmetic. */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for unsigned 64‑bit values, no floating point */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo + 1) >> 1);  /* ceil((lo+hi)/2) */
        if (mid <= x / mid) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Map the linear index n (1‑based) to row r and column k. */
    uint64_t tn = (uint64_t)n;          /* index of the term in the flattened array */
    uint64_t s = isqrt_uint64(8 * tn + 1);
    uint64_t r = (s - 1) >> 1;          /* candidate row */
    if (r * (r + 1) / 2 < tn) ++r;     /* adjust if necessary */

    uint64_t prev = r * (r - 1) / 2;    /* number of terms before row r */
    uint64_t k   = tn - prev;           /* column number (1‑based) */

    /* Count i in [1,2r] such that the fractional part lies in [(k‑1)/r , k/r). */
    uint64_t count = 0;
    for (uint64_t i = 1; i <= 2 * r; ++i) {
        uint64_t ai = isqrt_uint64(5 * i * i);           /* floor(i*√5)           */
        uint64_t ri = r * i;
        uint64_t bi = isqrt_uint64(5 * ri * ri);         /* floor(r*i*√5)         */
        if (bi == r * ai + (k - 1))
            ++count;
    }

    return (seq_t)count;
}
