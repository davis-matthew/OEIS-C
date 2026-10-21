/*
 * A211785  Rectangular array: R(n,k)=n^3+[(n^3)/2]+...+[(n^3)/k], where [
 *          ]=floor, by antidiagonals.
 * https://oeis.org/A211785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{j=1}^{k} floor(i^3 / j) where (i,k) are defined by
 *            the anti‑diagonal enumeration of the rectangular array R(n,k).
 *            The index n is found in block m with T_{m-1} < n <= T_m, T_m =
 *            m(m+1)/2. Inside block m the offset pos = n - T_{m-1} - 1
 *            determines (i,k): if pos == m-1 : (i,k) = (1, T_m)...
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, but return 0 */

    /* find block m such that T_{m-1} < n <= T_m, where T_m=m(m+1)/2 */
    idx_t m = (idx_t)((sqrtl(8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
    /* adjust in case of rounding errors */
    while (((__int128)m * (m + 1) / 2) < n) ++m;
    while (m > 0 && ((__int128)(m - 1) * m / 2) >= n) --m;

    /* triangular numbers (use 128‑bit to avoid overflow) */
    __int128 T_prev = (__int128)(m - 1) * m / 2;
    __int128 T_m    = (__int128)m * (m + 1) / 2;

    idx_t pos = (idx_t)(n - (idx_t)T_prev - 1);   /* 0‑based offset inside block */

    idx_t i, k;
    if (pos == m - 1) {
        i = 1;
        /* k = T_m, need to fit into idx_t */
        k = (idx_t)T_m;
    } else {
        i = m - pos;
        k = pos + 1;
    }

    seq_t n3 = (seq_t)i * i * i;  /* i^3 as seq_t */
    seq_t sum = 0;
    for (idx_t j = 1; j <= k; ++j) {
        sum += n3 / j;              /* floor division */
    }
    return sum;
}
