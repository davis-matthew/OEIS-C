/*
 * A183051  Array of least knight's moves to points on the square |i|+|j|=n on
 *          infinite chessboard.
 * https://oeis.org/A183051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = minimal knight moves from (0,0) to the nth point in the
 *            antidiagonal enumeration of all lattice points: start with
 *            (0,0); for k ≥ 1 enumerate the 4k points with |i|+|j|=k in the
 *            order
 *            (0,k),(1,k-1),…,(k,0),(k-1,-1),…,(0,-k),(-1,-k+1),…(-k,0),(-k+1,1),…(-1,k-1)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the minimal number of knight moves from (0,0) to (x,y). */
static seq_t knight_distance(int64_t x, int64_t y)
{
    if (x == 0 && y == 0)
        return 0;

    unsigned long long ax = (x >= 0) ? (unsigned long long)x : (unsigned long long)(-x);
    unsigned long long ay = (y >= 0) ? (unsigned long long)y : (unsigned long long)(-y);

    if (ax < ay) {
        unsigned long long t = ax;
        ax = ay;
        ay = t;
    }

    if (ax == 1 && ay == 0)
        return 3;
    if (ax == 2 && ay == 2)
        return 4;

    unsigned long long d1 = (ax + 1) / 2;
    unsigned long long d2 = (ax + ay + 2) / 3;
    unsigned long long d = d1 > d2 ? d1 : d2;

    if ((d & 1ULL) != ((ax + ay) & 1ULL))
        d++;

    return (seq_t)d;
}

/* Returns the cumulative number of points up to and including ring m. */
static inline idx_t cum_rows(idx_t m)
{
    return (idx_t)1 + (idx_t)2 * m * (m + 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* defined only for n ≥ 1 */

    idx_t m = 0;
    while (cum_rows(m) < n)
        ++m;

    idx_t prev_cum = (m == 0) ? 0 : cum_rows(m - 1);
    idx_t p = n - prev_cum - 1;          /* 0‑based index inside ring m */
    int64_t x = 0, y = 0;

    if (m == 0) {
        x = 0;
        y = 0;
    } else {
        if (p <= m) {                       /* Segment 1: (0,m) → (m,0) */
            x = (int64_t)p;
            y = (int64_t)m - (int64_t)p;
        } else if (p <= 2 * m) {            /* Segment 2: (m,0) → (0,−m) */
            idx_t q = p - m;                /* q ∈ [1, m] */
            x = (int64_t)m - (int64_t)q;    /* 2*m - p */
            y = -(int64_t)q;                /* -(p - m) */
        } else if (p <= 3 * m) {            /* Segment 3: (0,−m) → (−m,0) */
            idx_t r = p - 2 * m;            /* r ∈ [1, m] */
            x = -(int64_t)r;
            y = (int64_t)p - (int64_t)3 * m;
        } else {                            /* Segment 4: (−m,0) → (0, m) */
            idx_t s = p - 3 * m;            /* s ∈ [1, m‑1] */
            x = -((int64_t)m - (int64_t)s);
            y = (int64_t)s;
        }
    }

    return knight_distance(x, y);
}
