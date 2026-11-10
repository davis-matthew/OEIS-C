/*
 * A205876  Positions of multiples of 10 in A204922 (differences of Fibonacci
 *          numbers).
 * https://oeis.org/A205876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th multiple of 10 in A204922, where
 *            A204922 is the triangle T(r,c) = F(r+2) - F(c+1), 1<=c<=r, read
 *            by rows. Computed by a scan using Fibonacci numbers mod 10
 *            (period 60)
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int A205876_fm[60];
static bool A205876_init = false;

seq_t a(idx_t n) {
    if (!A205876_init) {
        A205876_fm[0] = 0;
        A205876_fm[1] = 1;
        for (int i = 2; i < 60; ++i)
            A205876_fm[i] = (A205876_fm[i - 1] + A205876_fm[i - 2]) % 10;
        A205876_init = true;
    }
    idx_t pos = 0, found = 0;
    for (idx_t r = 1; ; ++r) {
        int fr = A205876_fm[(r + 2) % 60];
        for (idx_t c = 1; c <= r; ++c) {
            ++pos;
            if (A205876_fm[(c + 1) % 60] == fr) {
                ++found;
                if (found == n) return (seq_t)pos;
            }
        }
    }
}
