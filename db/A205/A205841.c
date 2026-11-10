/*
 * A205841  Positions of multiples of 3 in A204922 (differences of Fibonacci
 *          numbers).
 * https://oeis.org/A205841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position m in A204922 of the n-th term divisible by 3,
 *            where A204922 is the triangle T(r,c) = F(r+2) - F(c+1), 1 <= c
 *            <= r, read by rows, and m = r*(r-1)/2 + c. Computed by a scan
 *            over (r,c), comparing Fibonacci residues mod 3
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const int fm[8] = {0, 1, 1, 2, 0, 2, 2, 1}; /* F(k) mod 3, period 8 */
    idx_t count = 0;
    for (idx_t r = 1; ; ++r) {
        int rr = fm[(r + 2) & 7U];
        for (idx_t c = 1; c <= r; ++c) {
            if (fm[(c + 1) & 7U] == rr) {
                ++count;
                if (count == n)
                    return (seq_t)(r * (r - 1) / 2 + c);
            }
        }
    }
}
