/*
 * A128138  A000012 * A128132.
 * https://oeis.org/A128138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the entry in the lower‑triangular matrix where T(n,n)=n
 *            and T(n,k)=k-1 for 0<k<n, with rows enumerated lexicographically
 *            (row 1, then row 2, …). For n=1 the value is 1
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case for the first term */
    if (n == 1) return (seq_t)1;

    /* Find the row r such that T_{r-1} < n ≤ T_r (T_r = r(r+1)/2) */
    idx_t r = 0;
    __int128 t = 0;                /* t = T_r      */
    while ((uint64_t)t < n) {      /* loop in 64‑bit to avoid overflow for small n */
        r++;
        t += r;                    /* t = t + r  →  t = r(r+1)/2   */
    }

    /* Column index within the row */
    idx_t k = n - (uint64_t)(t - r);   /* t - r = T_{r-1} */

    /* Compute the value according to the triangle rules */
    if (k == 1) return (seq_t)0;          /* first element of any non‑empty row */
    if (k == r) return (seq_t)r;          /* diagonal element */
    return (seq_t)(k - 1);                 /* other elements */
}
