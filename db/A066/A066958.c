/*
 * A066958  Number of ordered primitive solutions (x,y,z) to xy + yz + zx = n
 *          with x,y,z >= 1.
 * https://oeis.org/A066958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 181, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A066958: Number of ordered primitive solutions (x,y,z) to xy + yz + zx = n
 *  with x,y,z ≥ 1.  The routine enumerates all pairs (x,y) with x*y < n,
 *  solves for z = (n - xy)/(x+y), checks that z is integral and positive,
 *  and verifies that gcd(x,y,z) = 1.  The count of all such ordered triples
 *  is returned as a `seq_t`.  Complexity: O(n log n) per call (≈ n·H_n),
 *  easily fast enough for n up to 10 000.  No hard‑coded tables are used.
 */

#include <stddef.h>   /* for size_t if needed */

/* Euclidean algorithm for idx_t */
static idx_t
gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    seq_t count = 0;

    if (n < 3)          /* no solutions for n = 1,2 */
        return count;

    for (idx_t x = 1; x <= n; ++x) {
        /* y must satisfy x*y < n => y <= (n-1)/x */
        idx_t y_max = (n - 1) / x;
        for (idx_t y = 1; y <= y_max; ++y) {
            idx_t prod = x * y;               /* x*y < n always holds */
            seq_t N = (seq_t)n - prod;        /* numerator n - xy   */
            seq_t D = (seq_t)x + (seq_t)y;    /* denominator x + y */

            if (N % D != 0)
                continue;                     /* z not an integer */

            seq_t z = N / D;
            if (z < 1)
                continue;                     /* z must be positive */

            /* primitive check: gcd(x,y,z) == 1 */
            idx_t gxy  = gcd_idx(x, y);
            idx_t gcd_all = gcd_idx(gxy, (idx_t)z);
            if (gcd_all == 1)
                ++count;
        }
    }

    return count;
}
