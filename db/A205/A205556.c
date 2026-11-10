/*
 * A205556  Positions of multiples of 2 in A204922 (differences of Fibonacci
 *          numbers).
 * https://oeis.org/A205556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th position m in A204922 where A204922(m) =
 *            F(r+2)-F(c+1) is even * (m = r*(r-1)/2 + c, 1<=c<=r); F(k) is
 *            even iff 3|k, so it is even iff (3|(r+2)) == (3|(c+1))
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t pos = 0;
    for (idx_t r = 1; ; ++r) {
        bool pe = ((r + 2) % 3 == 0);
        for (idx_t c = 1; c <= r; ++c) {
            ++pos;
            bool pc = ((c + 1) % 3 == 0);
            if (pe == pc) {
                ++count;
                if (count == n) return (seq_t)pos;
            }
        }
    }
}
