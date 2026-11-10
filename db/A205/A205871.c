/*
 * A205871  Positions of multiples of 9 in A204922 (differences of Fibonacci
 *          numbers).
 * https://oeis.org/A205871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position m of the n-th multiple of 9 in A204922, where
 *            A204922(m) = F(r+2) - F(c+1), m = r(r-1)/2 + c, 1<=c<=r; found
 *            by scanning rows with Fibonacci residues mod 9 (Pisano period
 *            24)
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    int fm[24];
    fm[0] = 0;
    fm[1] = 1;
    for (int i = 2; i < 24; ++i) fm[i] = (fm[i - 1] + fm[i - 2]) % 9;

    idx_t count = 0;
    idx_t pos = 0;
    for (idx_t r = 1;; ++r) {
        int fr = fm[(r + 2) % 24];
        for (idx_t c = 1; c <= r; ++c) {
            ++pos;
            if (fm[(c + 1) % 24] == fr) {
                ++count;
                if (count == n) return (seq_t)pos;
            }
        }
    }
}
