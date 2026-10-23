/*
 * A177726  Number of line segments connecting exactly 10 points in an n x n
 *          grid of points.
 * https://oeis.org/A177726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of grid points in an n×n grid
 *            for which the gcd of the coordinate differences is 9 (i.e., the
 *            line segment contains exactly 10 lattice points)
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean gcd for unsigned indices */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* For n < 10 there cannot be a difference of 9 or more, so the result is 0. */
    if (n < 10)
        return 0;

    /* Maximum absolute coordinate difference allowed for a vector with gcd=9 */
    idx_t A = (n - 1) / 9;          /* |a| <= (n-1)/9 */
    idx_t B = A;                    /* symmetry, |b| <= A */

    seq_t sum = 0;

    /* Iterate over all integer differences (da, db) with |da| <= A, |db| <= B.
       The loop ranges from -A to +A to include negative differences automatically. */
    for (int da = -(int)A; da <= (int)A; ++da) {
        idx_t abs_da = da < 0 ? (idx_t)(-da) : (idx_t)da;
        seq_t term_da = (seq_t)(n) - (seq_t)9 * (seq_t)abs_da;  /* n - 9|da| */

        if (term_da == 0)
            continue;   /* No valid starting points for this |da| */

        for (int db = -(int)B; db <= (int)B; ++db) {
            if (da == 0 && db == 0)
                continue;   /* Skip the zero vector */

            idx_t abs_db = db < 0 ? (idx_t)(-db) : (idx_t)db;

            /* Only consider pairs where gcd(|da|,|db|) == 1
               (since |da|,|db| are scaled by 9 already). */
            if (gcd_idx(abs_da, abs_db) != 1)
                continue;

            seq_t term_db = (seq_t)(n) - (seq_t)9 * (seq_t)abs_db;  /* n - 9|db| */
            if (term_db == 0)
                continue;

            sum += term_da * term_db;
        }
    }

    /* Each unordered segment has been counted twice (once for each direction),
       so divide by 2. */
    return sum / 2;
}
