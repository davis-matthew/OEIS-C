/*
 * A121876  a(n) = number of solutions to the Diophantine equation
 *          x+y^2+z^3=n^4 with positive x,y,z.
 * https://oeis.org/A121876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of positive (x,y,z) with x + y^2 + z^3 = n^4
 *            Implemented by enumerating z and counting admissible y
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 214, u64 n <= 214,
 *            u128 n <= 214, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer cube root: largest r with r^3 <= val (val fits in idx_t) */
static idx_t int_cube_root(idx_t val)
{
    idx_t lo = 0, hi = val;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 cube = (unsigned __int128)mid * mid * mid;
        if (cube == val)
            return mid;
        if (cube < val)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* integer floor square root: largest r with r^2 <= val (val fits in idx_t) */
static idx_t floor_sqrt(idx_t val)
{
    idx_t lo = 0, hi = val;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == val)
            return mid;
        if (sq < val)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* compute n^4 as seq_t */
    seq_t n_seq = (seq_t)n;
    seq_t n2   = n_seq * n_seq;       /* n^2 */
    seq_t n4   = n2 * n2;             /* n^4 */

    /* convert to idx_t for integer bounds (n^4 <= 10^16 < 2^63) */
    idx_t n4_idx = (idx_t)n4;

    /* max z such that z^3 < n^4  →  z <= cube_root(n^4 - 1) */
    idx_t zmax = int_cube_root(n4_idx - 1);

    seq_t count = 0;

    for (idx_t z = 1; z <= zmax; ++z) {
        seq_t zcube = (seq_t)z * (seq_t)z * (seq_t)z; /* z^3 */
        if (zcube >= n4)
            continue;              /* safety, should not happen */

        seq_t remainder = n4 - zcube;                /* n^4 - z^3 */

        if (remainder <= (seq_t)1)                  /* need y^2 < remainder */
            continue;

        /* y must satisfy 1 <= y < sqrt(remainder)   →   ymax = floor(sqrt(remainder - 1)) */
        idx_t ymax = floor_sqrt((idx_t)(remainder - 1));
        count += (seq_t)ymax;
    }
    return count;
}
