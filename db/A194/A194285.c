/*
 * A194285  Triangular array: g(n,k)=number of fractional parts (i*sqrt(2)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n, 1<=k<=n.
 * https://oeis.org/A194285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 96, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the triangular array A194285. For a given flat-index n (starting at 1),
 * the function returns the value g(r,k), where r is the row and k the column in
 * the array defined by
 *
 *   g(r,k) = # { i : 1 ≤ i ≤ r  and  (k-1)/r ≤ frac(i·√2) < k/r }.
 *
 * The sequence is therefore the row‑major flattening of that array. */

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include "../utils/floor_sqrt.h"  /* floor_sqrt(seq_t) */

static inline seq_t g_count(idx_t r, idx_t k)
{
    seq_t cnt = 0;
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;
    for (idx_t i = 1; i <= r; ++i) {
        /* integer floor of i·√2 using the identity
         *   floor(i·√2) = floor( sqrt( 2·i² ) )                     */
        seq_t t   = (seq_t)2 * (seq_t)i * (seq_t)i;
        seq_t mi  = floor_sqrt(t);              /* floor(i·√2) */

        /* bounds expressed as rational numbers m + (k-1)/r  and  m + k/r   */
        seq_t p1  = mi * rr + (kk - 1);          /* m·r + (k‑1)  */
        seq_t p2  = mi * rr +  kk;              /* m·r +  k    */

        seq_t left = (seq_t)2 * (seq_t)i * (seq_t)i * rr * rr;

        /* left >= p1²  and  left < p2²  ⇔  fractional part lies in [ (k-1)/r , k/r ) */
        if (left >= p1 * p1 && left < p2 * p2) {
            cnt += 1;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but keep safe */

    /* Find smallest r with r*(r+1)/2 >= n (row index) */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 s = (__int128)mid * (mid + 1);
        s /= 2;
        if (s >= n) high = mid;
        else        low  = mid + 1;
    }
    idx_t r = low;

    /* Column index: k = n - r*(r-1)/2  */
    idx_t offset = (r * (r - 1)) / 2;
    idx_t k = n - offset;

    return g_count(r, k);
}
