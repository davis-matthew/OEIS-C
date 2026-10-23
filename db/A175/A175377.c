/*
 * A175377  a(n) = A175376(n^2).
 * https://oeis.org/A175377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer triples (x,y,z) satisfying * x^4 + y^4
 *            + z^4 <= n^2, -n <= x,y,z <= n * Algorithm: iterate over
 *            non‑negative x,y. For each pair compute the * remaining bound
 *            for |z|. The maximum |z| is found by binary search * on a
 *            pre‑computed table of i^4 (0 <= i <= n). Symmetries in x...
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Precompute i^4 for i=0..n */
    size_t size = (size_t)n + 1;
    seq_t *pow4 = (seq_t *)malloc(size * sizeof(seq_t));
    if (!pow4) return 0; /* allocation failure should never happen */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t ii2 = ii * ii;      /* i^2 */
        pow4[i] = ii2 * ii2;      /* i^4 */
    }

    seq_t nsq = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t total = 0;

    /* Iterate over x >= 0 */
    for (idx_t xi = 0; xi <= n; ++xi) {
        seq_t x4 = pow4[xi];
        if (x4 > nsq) break;            /* no further x will satisfy the bound */
        idx_t mult_x = (xi == 0 ? 1 : 2);

        /* iterate over y >= 0 */
        for (idx_t yi = 0; yi <= n; ++yi) {
            seq_t sum_xy = x4 + pow4[yi];
            if (sum_xy > nsq) break;    /* further y will only increase the sum */
            idx_t mult_y = (yi == 0 ? 1 : 2);

            /* remaining bound for z^4 */
            seq_t rem = nsq - sum_xy;

            /* binary search for largest i with i^4 <= rem */
            idx_t lo = 0, hi = (idx_t)n;
            while (lo <= hi) {
                idx_t mid = (lo + hi) >> 1;
                if (pow4[mid] <= rem) {
                    lo = mid + 1;
                } else {
                    if (mid == 0) { hi = 0; break; }
                    hi = mid - 1;
                }
            }
            idx_t z_abs = lo - 1;          /* floor fourth root of rem */
            seq_t count_z = (seq_t)(2 * z_abs + 1);

            seq_t add = count_z * (seq_t)(mult_x * mult_y);
            total += add;
        }
    }

    free(pow4);
    return total;
}
