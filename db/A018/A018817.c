/*
 * A018817  Number of lines through exactly 10 points of an n X n grid of
 *          points.
 * https://oeis.org/A018817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct lines in an n×n integer grid that
 *            contain exactly 10 grid points. Algorithm: * Enumerate all
 *            primitive direction vectors (dx,dy) with gcd(dx,dy)=1, where
 *            dx≥0, dy≥0, and not both zero. * For each direction, compute the
 *            number of lines having exactly 10 lattice points usi...
 * Range:     u8 n <= 28, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* No lines can contain 10 points for n < 10 */
    if (n < 10)
        return 0;

    idx_t maxd = (n - 1) / 9;          /* maximal |dx| or |dy| to consider */
    seq_t total = 0;

    for (idx_t dx = 0; dx <= maxd; ++dx) {
        idx_t Lx = n - 9 * dx;         /* must be > 0 by construction */
        if (Lx == 0)
            continue;

        for (idx_t dy = 0; dy <= maxd; ++dy) {
            if (dx == 0 && dy == 0)
                continue;
            if (gcd_idx(dx, dy) != 1)
                continue;

            idx_t Ly = n - 9 * dy;
            if (Ly == 0)
                continue;

            idx_t a_val = n >= 10 * dx ? n - 10 * dx : 0;
            idx_t b_val = n >= 10 * dy ? n - 10 * dy : 0;
            idx_t c_val = n >= 11 * dx ? n - 11 * dx : 0;
            idx_t d_val = n >= 11 * dy ? n - 11 * dy : 0;

            seq_t LxS = (seq_t)Lx;
            seq_t LyS = (seq_t)Ly;
            seq_t aS   = (seq_t)a_val;
            seq_t bS   = (seq_t)b_val;
            seq_t cS   = (seq_t)c_val;
            seq_t dS   = (seq_t)d_val;

            seq_t count = LxS * LyS - 2 * aS * bS + cS * dS;

            if (dx == 0 && dy > 0)          /* vertical lines */
                total += count;
            else if (dy == 0 && dx > 0)     /* horizontal lines */
                total += count;
            else if (dx > 0 && dy > 0)      /* positive slope: double for negative */
                total += count * 2;
            /* (dx==0 && dy==0) already skipped */
        }
    }

    return total;
}
