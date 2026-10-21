/*
 * A225951  Triangle for perimeters of primitive Pythagorean triangles.
 * https://oeis.org/A225951
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) enumerates the entries of the triangle a(n,m) in row‑major
 *            order, * where * a(n,m) = 2*n*(n+m) if n>m≥1, gcd(n,m)=1 and n+m
 *            is odd, * a(n,m) = 0 otherwise. * * a(2) is the first term of
 *            the sequence (offset 2). The function returns * the value at
 *            index n≥2; for n<2 the result is 0. * * The i...
 * Range:     u8 n <= 36, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */

/* Integer square root for unsigned 64-bit values, floor.  Small, fast. */
static idx_t isqrt_u64(idx_t v)
{
    idx_t r = 0;
    for (int shift = 31; shift >= 0; shift--) {
        idx_t cand = r | ((idx_t)1 << shift);
        if (cand * cand <= v)
            r = cand;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    idx_t e = n - 2;                       /* zero‑based position in the list */
    /* Compute row r (r ≥ 2) such that: (r-1)*(r-2)/2 ≤ e < r*(r-1)/2. */
    idx_t sq = 1 + 8 * e;                  /* 1 + 8e  (no overflow for given bounds) */
    idx_t s = isqrt_u64(sq);              /* floor sqrt(1+8e) */
    idx_t r = (1 + s) / 2 + 1;             /* minimal r with e < r*(r-1)/2 */

    /* Calculate offset within the row r. */
    idx_t tPrev = (r - 2) * (r - 1) / 2;   /* entries before row r */
    idx_t pos = e - tPrev;                 /* 0‑based offset in row r */
    idx_t m = pos + 1;                     /* column index (1 ≤ m < r) */

    /* Condition: r>m (always true here), gcd(r,m)=1, and r+m odd. */
    if ((r + m) & 1) {                     /* opposite parity */
        seq_t g = gcd((seq_t)r, (seq_t)m);
        if (g == (seq_t)1) {
            /* Compute 2*r*(r+m) in seq_t. */
            seq_t val = ((seq_t)2 * (seq_t)r * ((seq_t)r + (seq_t)m));
            return val;
        }
    }
    return (seq_t)0;
}
