/*
 * A070771  b+c+d+e where b>=c>=d>=e>=0 ordered by b then c then d then e.
 * https://oeis.org/A070771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the lexicographically n‑th 4‑tuple (b, c, d, e)
 *            with b >= c >= d >= e >= 0, where tuples are ordered by
 *            successive coordinates. The function returns b+c+d+e
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static inline idx_t binom2(idx_t n) {
    /* n choose 2 */
    return n * (n - 1) / 2;
}

static inline idx_t binom3(idx_t n) {
    /* n choose 3 */
    return n * (n - 1) * (n - 2) / 6;
}

seq_t a(idx_t n) {
    /* Find b such that cumulative count of tuples with first coordinate
       <= b exceeds n.  Count for a fixed b is C(b+3, 3). */
    idx_t cum = 0;
    idx_t b = 0;
    while (1) {
        idx_t count = binom3(b + 3);   /* C(b+3, 3) */
        if (cum + count > n) break;
        cum += count;
        b++;
    }
    idx_t offset = n - cum;   /* position inside block of given b */

    /* Find c within the block of fixed b.  Count for a fixed c is
       C(c+2, 2). */
    idx_t ccum = 0;
    idx_t c = 0;
    while (1) {
        idx_t count = binom2(c + 2);   /* C(c+2, 2) */
        if (ccum + count > offset) break;
        ccum += count;
        c++;
    }
    idx_t offset2 = offset - ccum;   /* position inside block of given c */

    /* Find d within the block of fixed b,c.  For a fixed d there are
       d+1 possibilities for e (0..d). */
    idx_t d = 0;
    idx_t dcum = 0;
    for (; d <= c; ++d) {
        idx_t count = d + 1;
        if (dcum + count > offset2) break;
        dcum += count;
    }
    idx_t offset3 = offset2 - dcum;   /* e index within [0..d] */

    idx_t e = offset3;   /* e value */

    return (seq_t)(b + c + d + e);
}
