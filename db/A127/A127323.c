/*
 * A127323  Third 4-dimensional hyper-tetrahedral coordinate; 4-D analog of
 *          A056557.
 * https://oeis.org/A127323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the third coordinate Y in the 4‑D hyper‑tetrahedral ordering
 * of all non‑negative 4‑tuples (W, X, Y, Z) with W ≥ X ≥ Y ≥ Z.
 *
 * The indexing is
 *
 *   n = C(W+3, 4) + C(X+2, 3) + C(Y+1, 2) + Z
 *
 * where C(a,b) = a choose b.  This function inverts the mapping
 * to find the unique Y that corresponds to the given n.
 *
 * The algorithm uses only integer arithmetic and works for any
 * build that defines seq_t (unsigned integer or arbitrary‑precision)
 * and idx_t (uint64_t).  All temporary computations are performed in
 * seq_t to avoid overflow before casting back to idx_t when needed.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* ---------- Find W --------------------------- */
    idx_t w = 0;
    while (1) {
        idx_t m = w + 3;
        /* C(m,4) with seq_t arithmetic */
        seq_t startW = (seq_t)m * (m - 1) * (m - 2) * (m - 3) / 24;
        if (startW > (seq_t)n) break;
        ++w;
    }
    --w; /* last iteration overshot */
    idx_t m = w + 3;
    seq_t startW = (seq_t)m * (m - 1) * (m - 2) * (m - 3) / 24;
    seq_t rn = (seq_t)n;
    seq_t r1 = rn - startW;     /* remainder within the W‑block */

    /* ---------- Find X --------------------------- */
    idx_t x = 0;
    while (1) {
        idx_t t = x + 2;
        seq_t offsX = (seq_t)t * (t - 1) * (t - 2) / 6;   /* C(t,3) */
        if (offsX > r1) break;
        ++x;
    }
    --x;
    idx_t t = x + 2;
    seq_t offsX = (seq_t)t * (t - 1) * (t - 2) / 6;   /* compute again */
    seq_t r2 = r1 - offsX;     /* remainder within the X‑block */

    /* ---------- Find Y --------------------------- */
    idx_t y = 0;
    while ( (seq_t)(y + 2) * (y + 1) / 2 <= r2 ) {
        ++y;
    }
    /* At this point y is the largest value with C(y+2,2) ≤ r2,
       which means Y = y. */
    return (seq_t)y;
}
