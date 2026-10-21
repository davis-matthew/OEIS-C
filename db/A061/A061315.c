/*
 * A061315  Array read by antidiagonals: T(n,k)=T(n,k-1)*(T(n,k-1)+k-1)/k with
 *          T(n,1)=n.
 * https://oeis.org/A061315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 30, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A061315 – array read by antidiagonals
 *
 *  T(n,1) = n
 *  T(n,k) = T(n,k-1) * (T(n,k-1) + k-1) / k   (k > 1)
 *
 *  a(n) is obtained by reading the elements T(r,c) in order of
 *  increasing antidiagonal index, starting with a(1)=T(1,1).
 *
 *  This implementation finds the position (r,c) corresponding to
 *  n and evaluates T(r,c) iteratively.  Intermediate 128‑bit
 *  arithmetic is used so that all values that fit into seq_t
 *  are computed exactly.  The routine works for any seq_t
 *  and idx_t types defined by the build. */

#include <stdint.h>  /* for uint64_t and __int128 support */

seq_t a(idx_t n)
{
    /* a(1) is the first term – for n < 1 the sequence is undefined */
    if (n < 1)
        return (seq_t)0;

    /* --- find the anti‑diagonal `d` that contains the n‑th element --- */
    idx_t d = 1;
    while ((unsigned __int128)d * (d + 1) / 2 < n)
        ++d;                       /* `d` is now the diagonal index */

    /* pref(d-1) = (d-1)*d/2   – number of terms before diagonal `d` */
    idx_t pref_prev = (d - 1) * d / 2;
    /* zero‑based position within diagonal `d` */
    idx_t pos = n - pref_prev - 1;

    /* coordinates in the table */
    idx_t r = pos + 1;    /* row index, 1‑based */
    idx_t c = d - pos;    /* column index, 1‑based */

    /* --- evaluate T(r,c) iteratively --- */
    seq_t val = (seq_t)r;            /* T(r,1) */
    for (idx_t k = 2; k <= c; ++k) {
        /* use 128‑bit arithmetic for the intermediate product */
        unsigned __int128 v = (unsigned __int128)val;
        unsigned __int128 prod = v * (v + (unsigned __int128)(k - 1));
        val = (seq_t)(prod / k);
    }

    return val;
}
