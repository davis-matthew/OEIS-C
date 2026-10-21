/*
 * A090930  Permutation of natural numbers arising from a square spiral.
 * https://oeis.org/A090930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = value obtained by traversing the counter‑clockwise *
 *            square spiral in rings. For each ring r (r ≥ 1) the sequence *
 *            lists the numbers on that ring in clockwise order, starting at
 *            the * cell (−(r−1), r). The spiral numbering itself follows the
 *            usual * counter‑clockwise pattern described in...
 * Range:     u8 n <= 240, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)      /* the central cell */
        return (seq_t)1;

    /* find the ring r that contains position n */
    idx_t r = 0;
    while (1) {
        __int128 last = (__int128)1 + (__int128)4 * r * (r + 1);
        if ((__int128)n <= last)
            break;
        r++;
    }

    /* first index of this ring */
    idx_t first = (idx_t)2 + (idx_t)4 * (r - 1) * r;
    idx_t offset = n - first;      /* 0‑based within the ring       */
    idx_t side  = (idx_t)2 * r;    /* length of each side           */
    idx_t seg   = offset / side;   /* 0 top, 1 right, 2 bottom, 3 left  */
    idx_t a_off = offset % side;

    /* coordinates (x,y) of the cell in the a(n) order */
    int64_t x, y;
    switch (seg) {
        case 0:                     /* top side, moving right     */
            x = -(int64_t)(r - 1) + (int64_t)a_off;
            y = (int64_t)r;
            break;
        case 1:                     /* right side, moving down    */
            x = (int64_t)r;
            y = (int64_t)r - 1 - (int64_t)a_off;
            break;
        case 2:                     /* bottom side, moving left   */
            x = (int64_t)(r - 1) - (int64_t)a_off;
            y = -(int64_t)r;
            break;
        case 3:                     /* left side, moving up       */
            x = -(int64_t)r;
            y = -(int64_t)r + 1 + (int64_t)a_off;
            break;
        default:                     /* never reached                */
            x = y = 0;
            break;
    }

    /* compute the number assigned to (x,y) in the standard spiral */
    int64_t m = r;                     /* = max(|x|,|y|) for ring r */
    seq_t t = (seq_t)(2 * m + 1);
    t = t * t;                         /* (2m+1)^2 */

    seq_t d;
    if (x == m) {
        d = (seq_t)(m - y);
    } else if (y == -(int64_t)m) {
        d = (seq_t)(2 * m + (m - x));
    } else if (x == -(int64_t)m) {
        d = (seq_t)(4 * m + (y + m));
    } else { /* y == m */
        d = (seq_t)(6 * m + (x + m));
    }

    return t - d;
}
