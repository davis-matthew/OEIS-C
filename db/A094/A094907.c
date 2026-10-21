/*
 * A094907  Number of different nontrivial two-digit cancellations of the form
 *          (xy)/(zx) = y/z in base n.
 * https://oeis.org/A094907
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct triples of non‑zero digits (x,y,z)
 *            with 1 ≤ x,y,z < n and x > y for which (x*n + y)/(z*n + x) =
 *            y/z. This is found by a brute‑force search over all admissible
 *            digit pairs (x,y). Non‑trivial cases are automatically excluded
 *            because the only trivial solution would require...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;

    if (n < 3)   /* for n = 2,3 the sequence is 0 */
        return 0;

    /* iterate over leading digits x (numerator tens) and y (numerator ones) */
    for (idx_t x = 2; x < n; ++x) {
        for (idx_t y = 1; y < x; ++y) {
            /* numerator = x*y (positive integer) */
            int64_t num   = (int64_t)x * (int64_t)y;
            /* denominator = n*(x-y) + y  (positive when x>y) */
            int64_t denom = (int64_t)n * (int64_t)(x - y) + (int64_t)y;

            if (denom == 0)
                continue;

            if (num % denom == 0) {
                int64_t z = num / denom;
                if (z >= 1 && z < (int64_t)n)
                    ++count;
            }
        }
    }

    return count;
}
