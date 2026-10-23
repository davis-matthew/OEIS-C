/*
 * A143974  Rectangular array R by antidiagonals: label each unit square in
 *          the first quadrant lattice by its northeast vertex (x,y) and mark
 *          those having x+y=1(mod 3); then R(m,n) is the number of marked
 *          unit squares in the rectangle [0,m]x[0,n].
 * https://oeis.org/A143974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ⌊((m+1)*(n+1))/3⌋ where (m,n) are the coordinates of the
 *            n‑th entry in the anti‑diagonal enumeration of the array R. The
 *            anti‑diagonal enumeration starts at (0,0) and lists all pairs
 *            (x,y) with x+y = k in order k=0,1,2,… . Indices are 1‑based:
 *            a(1) corresponds to (0,0)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined for n=0, defensive */

    /* ---- find k such that Σ_{i=0}^{k} (i+1) = (k+1)(k+2)/2 >= n ---- */
    idx_t low = 0, high = 1;

    /* grow upper bound so that it is at least n */
    while (((__int128)(high + 1) * (high + 2)) / 2 < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 prod = (__int128)(mid + 1) * (mid + 2);
        idx_t c = (idx_t)(prod / 2);        /* cumulative up to mid */
        if (c >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;                       /* diagonal index */

    /* ---- compute offset within diagonal ---- */
    idx_t prev_c = (idx_t)(((__int128)k * (k + 1)) / 2);
    idx_t offset = n - prev_c - 1;       /* 0‑based offset on diagonal */

    /* coordinates: (x,y) = (k - offset, offset) */
    seq_t xm = (seq_t)(k - offset);
    seq_t ym = (seq_t)offset;

    /* ---- compute floor(((xm+1)*(ym+1))/3) ---- */
    seq_t prod = (xm + 1) * (ym + 1);
    return prod / 3;
}
