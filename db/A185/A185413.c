/*
 * A185413  A000027 written clockwise as spiral and read counterclockwise.
 * https://oeis.org/A185413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = value at the n‑th position of a counter‑clockwise spiral,
 *            where the natural numbers 1,2,3,… are arranged in a clockwise
 *            spiral starting at the origin (1 at (0,0)). The function finds
 *            the coordinates of the n‑th counter‑clockwise position and
 *            returns the number assigned to that coordinate b...
 * Range:     u8 n <= 240, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Validated: standard, pass, 80 terms through n = 985 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static inline seq_t cw_value(long long x, long long y) {
    /* absolute values */
    long long absx = x >= 0 ? x : -x;
    long long absy = y >= 0 ? y : -y;
    long long k = absx > absy ? absx : absy;   /* ring index */

    if (k == 0)         /* the center */
        return (seq_t)1;

    /* start of the ring */
    unsigned long long start = (unsigned long long)(2 * k - 1) *
                               (unsigned long long)(2 * k - 1) + 1;

    unsigned long long offset;
    if (x == k && y != -k) {                       /* right side (excluding bottom corner) */
        offset = (unsigned long long)(y + (k - 1));
    } else if (y == k) {                           /* top side */
        offset = (unsigned long long)(2 * k + (k - 1 - x));
    } else if (x == -k) {                          /* left side */
        offset = (unsigned long long)(4 * k - 1 + (k - y));
    } else {                                       /* bottom side */
        offset = (unsigned long long)(6 * k - 1 + (x + k));
    }

    return (seq_t)(start + offset);
}

seq_t a(idx_t n) {
    if (n == 0)              /* undefined but guard against underflow */
        return (seq_t)0;

    long long x = 1, y = 0;  /* first position of the counter‑clockwise spiral */
    idx_t rem = n - 1;        /* steps to perform from the starting point */

    if (rem == 0)
        return cw_value(x, y);

    const int dx[4] = { -1, 0, 1, 0 };  /* left  , up , right, down */
    const int dy[4] = {  0, 1, 0, -1 };

    int dir = 0;          /* start with left */
    int step_len = 1;     /* initial step length */

    while (rem > 0) {
        for (int d = 0; d < 4 && rem > 0; ++d) {
            idx_t moves = step_len;
            if (rem <= moves) {                /* finish inside this segment */
                x += (long long)dx[dir] * rem;
                y += (long long)dy[dir] * rem;
                rem = 0;
                break;
            } else {                           /* move full segment */
                x += (long long)dx[dir] * moves;
                y += (long long)dy[dir] * moves;
                rem -= moves;
            }
            dir = (dir + 1) & 3;                /* next direction */
            if (d == 1 || d == 3)               /* increase step length every two directions */
                ++step_len;
        }
    }

    return cw_value(x, y);
}
