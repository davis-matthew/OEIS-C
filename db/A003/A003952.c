/*
 * A003952  Expansion of g.f.: (1+x)/(1-9*x).
 * https://oeis.org/A003952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*9^(n-1) for n >= 1, a(0) = 1 (equivalently a(n) =
 *            (10*9^n - 0^n)/9)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   a003952_pow9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a003952_pow9.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    return (seq_t)10 * a003952_pow9(n - 1);
}
