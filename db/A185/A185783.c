/*
 * A185783  Second accumulation array of A185780, by antidiagonals.
 * https://oeis.org/A185783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(k+2,3) * C(n+1,2) * (k*n - n + 2*k + 4) / 6 where (n,k)
 *            are the coordinates of the n‑th term along the anti‑diagonals
 *            (row = n, col = k). See OEIS A185783
 * Range:     u8 n <= 16, u16 n <= 124, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root: floor(sqrt(x)) for 0 <= x < 2^64.  */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = 0xFFFFFFFFULL;          /* sqrt(2^64-1) = 4294967295 */
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid <= x / mid)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;    /* undefined for n<1 */

    /* ---------- Find the diagonal index s such that
       (s-1)*s/2 < n <= s*(s+1)/2.  ----------
       Use the integer square‑root.  */
    idx_t t = (idx_t)((__int128)1 + (__int128)8 * n);
    idx_t sqrt_t = isqrt_u64(t);
    idx_t s = (1 + sqrt_t) >> 1;          /* floor((1+sqrt(1+8n))/2) */
    while ((s - 1) * s / 2 < n) ++s;      /* adjust up if needed */
    /* ---------- End diagonal determination ---------- */

    /* Previous cumulative count up to diagonal s-1 */
    idx_t prev_cum = (s - 2) * (s - 1) / 2;
    /* 0‑based offset within the current diagonal */
    idx_t offset = n - prev_cum - 1;
    idx_t row   = offset + 1;            /* n in T(n,k) */
    idx_t col   = s - row;               /* k in T(n,k) */

    /* ---------- Compute the binomial coefficients ----------
       Use 128‑bit intermediates to avoid overflow for small seq_t.  */
    __int128 t1 = (__int128)(col + 2) * (col + 1) * col;          /* (k+2)(k+1)k */
    __int128 t2 = (__int128)(row + 1) * row;                     /* (n+1)n */
    __int128 expr = (__int128)col * row - row + 2 * col + 4;      /* k*n - n + 2k + 4 */

    __int128 bin1 = t1 / 6;   /* C(k+2,3) */
    __int128 bin2 = t2 / 2;   /* C(n+1,2) */

    __int128 prod  = bin1 * bin2 * expr;    /* intermediate product */
    prod /= 6;                              /* final division by 6 */

    return (seq_t)prod;
}
