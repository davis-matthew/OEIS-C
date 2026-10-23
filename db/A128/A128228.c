/*
 * A128228  A128229 * A002260.
 * https://oeis.org/A128228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r * k (k < r) or a(n) = r (k == r) where r is the unique
 *            integer such that r*(r+1)/2 ≥ n and k = n – r*(r‑1)/2. This
 *            produces the row‑major enumeration of the triangle T(n,k) = k*n
 *            (1≤k<n), T(n,n)=n
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* idx_t is uint64_t; seq_t is an unsigned integer type
   (uint8_t…uint64_t, unsigned __int128, or arbitrary‑precision). */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used – offset is 1 */

    /* Find row r such that r*(r+1)/2 ≥ n.  Binary search on [1,n]. */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* row number */
    unsigned __int128 prev128 = (unsigned __int128)r * (r - 1) / 2;
    idx_t prev = (idx_t)prev128;        /* number of elements before row r */
    idx_t k = n - prev;                 /* column index in row r (1‑based) */

    if (k == r)                         /* diagonal element */
        return (seq_t)r;
    else
        return (seq_t)r * k;            /* off‑diagonal element */
}
