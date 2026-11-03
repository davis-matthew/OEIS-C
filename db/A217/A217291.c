/*
 * A217291  Permutation of natural numbers arising from applying the walk of
 *          right triangular type-1 spiral (defined in A214230) to the data of
 *          square spiral (e.g. A214526).
 * https://oeis.org/A217291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = square_spiral_value_at( tri_coord(n) ) (permutes the
 *            square spiral by following the right‑triangular type‑1 spiral
 *            from A214230)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Map a coordinate (x,y) in the square spiral to its number */
static seq_t square_spiral_value_at(long long x, long long y)
{
    if (x == 0 && y == 0) return (seq_t)1;

    idx_t ax = x >= 0 ? (idx_t)x : (idx_t)(-x);
    idx_t ay = y >= 0 ? (idx_t)y : (idx_t)(-y);
    idx_t l  = (ax > ay) ? ax : ay;              /* layer */
    seq_t t  = (seq_t)(2 * l + 1) * (seq_t)(2 * l + 1);  /* max value in layer */

    long long kl = (long long)l;

    /* bottom‑right corner */
    if (x == kl && y == -kl) return t;

    /* top side (exclude top‑left corner) */
    if (y == kl && x != -kl)
        return t - ((seq_t)4 * l + (seq_t)(x + l));

    /* left side (includes top‑left corner) */
    if (x == -kl)
        return t - (seq_t)(2 * l) - (seq_t)(l + y);

    /* right side (includes bottom‑right corner already handled) */
    if (x == kl)
        return t - ((seq_t)7 * l - (seq_t)y);

    /* bottom side */
    return t - (seq_t)(l - x);
}

/* cumulative number of steps after completing segment s
   (segment 0 has length 1, segment s ≥ 1 has length s+1) */
static inline uint64_t steps_of_segments(uint64_t s)
{
    return (s + 1) * (s + 2) / 2;
}

/* coordinates used for the triangular walk */
typedef struct { long long x; long long y; } coord_t;

/* compute coordinates after (n‑1) steps of the triangular walk */
static coord_t tri_coord(idx_t n)
{
    if (n <= 1) return (coord_t){0, 0};

    uint64_t k = n - 1ull;                 /* steps to perform (≥1) */

    /* locate the segment that contains step k */
    uint64_t low = 0, high = 1;
    while (steps_of_segments(high) < k) high <<= 1;
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        if (steps_of_segments(mid) >= k) high = mid;
        else                            low = mid + 1;
    }
    uint64_t seg = low;                    /* current segment index */

    /* steps completed before the current segment */
    uint64_t moves_before = (seg == 0) ? 0 : seg * (seg + 1) / 2;
    uint64_t offset = k - moves_before;    /* 1‑based offset within the segment */

    long long x = 0, y = 0;
    for (uint64_t s = 0; s < seg; ++s) {
        long long dx, dy;
        uint64_t len = (s == 0) ? 1 : s + 1;
        if (s == 0) { dx = 1; dy = 0; }
        else {
            uint64_t dir = (s - 1) % 3;
            if (dir == 0) { dx = -1; dy = 1; }       /* up‑left */
            else if (dir == 1) { dx = 0; dy = -1; }   /* down   */
            else { dx = 1; dy = 0; }                 /* right  */
        }
        x += dx * (long long)len;
        y += dy * (long long)len;
    }

    /* add the remaining steps in the current segment */
    long long dx, dy;
    if (seg == 0) { dx = 1; dy = 0; }
    else {
        uint64_t dir = (seg - 1) % 3;
        if (dir == 0) { dx = -1; dy = 1; }
        else if (dir == 1) { dx = 0; dy = -1; }
        else { dx = 1; dy = 0; }
    }
    x += dx * (long long)offset;
    y += dy * (long long)offset;

    return (coord_t){x, y};
}

/* Main sequence function */
seq_t a(idx_t n)
{
    coord_t c = tri_coord(n);
    return square_spiral_value_at(c.x, c.y);
}
