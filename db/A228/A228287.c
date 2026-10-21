/*
 * A228287  Smallest value of z in the minimal value of x + y*z, given x*y + z
 *          = n (where x, y, z are positive integers).
 * https://oeis.org/A228287
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest z that attains the minimum of x + y*z over all
 *            positive integer triples satisfying x*y + z = n. A brute‑force
 *            search over all ordered divisor pairs (x,y) is performed
 * Range:     u8 n <= 31, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;           /* offset; behaviour for n < 2 is irrelevant */

    seq_t best_f = 0;        /* minimal value of x + y*z */
    idx_t best_z = 0;        /* z that attains it (smallest if tie) */
    bool first = true;       /* first candidate flag */

    /* Enumerate all ordered pairs (x, y) with x * y ≤ n – 1,
       guaranteeing positive z = n – x*y. */
    for (idx_t x = 1; x < n; ++x) {
        idx_t max_y = (n - 1) / x;          /* ensures z ≥ 1 */
        for (idx_t y = 1; y <= max_y; ++y) {
            idx_t z = n - x * y;            /* positive by construction */
            seq_t f = (seq_t)x + (seq_t)y * (seq_t)z;

            if (first || f < best_f || (f == best_f && z < best_z)) {
                best_f = f;
                best_z = z;
                first  = false;
            }
        }
    }

    return (seq_t)best_z;
}
