/*
 * A039055  Numbers whose base-8 representation has the same number of 0's and
 *          6's.
 * https://oeis.org/A039055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the smallest integer whose base‑8 representation has *
 *            equal numbers of digits 0 and 6, and which is the n‑th such
 *            integer * (n >= 1)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (uint64_t x = 1;; ++x) {
        unsigned zeros = 0, sixes = 0;
        uint64_t t = x;
        /* count 0's and 6's in base‑8 (using shift and mask) */
        while (t) {
            unsigned d = (unsigned)(t & 7);   /* t % 8 */
            if (d == 0)   ++zeros;
            else if (d == 6) ++sixes;
            t >>= 3;     /* t /= 8 */
        }
        if (zeros == sixes) {
            ++found;
            if (found == n) return (seq_t)x;
        }
    }
}
