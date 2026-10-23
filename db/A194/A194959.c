/*
 * A194959  Fractalization of (1 + floor(n/2)).
 * https://oeis.org/A194959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the values of the infinite array * A(i,j) =
 *            2*j-1 for j ≤ i, A(i,j) = 2*i otherwise. * The array is read by
 *            antidiagonals with decreasing i. * For a given index n (1‑based)
 *            we find the unique pair (i,j) * that occurs at position n and
 *            return A(i,j)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required by the build: seq_t and idx_t are typedef’ed elsewhere. */

#include <stdint.h>

/* Helper: triangular number T(s) = (s-1)*s/2, computed with 128‑bit to avoid overflow. */
static inline __int128 triangular(idx_t s)
{
    return (__int128)(s - 1) * s / 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for off‑by‑one callers */

    /* find minimal s such that triangular(s) >= n */
    idx_t low  = 1;
    idx_t high = 2;
    while (triangular(high) < ( __int128)n) {
        if (high > UINT64_MAX / 2) {       /* avoid overflow during doubling */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;                         /* high *= 2 */
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= ( __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                           /* s is the antidiagonal index */

    /* compute offset within this antidiagonal */
    idx_t preceding = (idx_t)(triangular(s - 1)); /* elements before sum s */
    idx_t offset = n - preceding - 1;            /* 0‑based offset in this diag */

    idx_t i = (s - 1) - offset;  /* row index (1‑based) */
    idx_t j = offset + 1;        /* column index (1‑based) */

    if (j <= i)
        return (seq_t)(2 * j - 1);   /* odd value */
    else
        return (seq_t)(2 * i);       /* even plateau value */
}
