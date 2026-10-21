/*
 * A202274  Numbers k for which sigma(k) = 2^m - 1 for some m.
 * https://oeis.org/A202274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n‑1) for 1 ≤ n ≤ 5, a(6)=25, a(n) = 2^(n‑2) for n ≥ 7
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   pow2_loop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS A202274: numbers k such that sigma(k) is a Mersenne number. */

#include <stdbool.h>
#include "../utils/pow2_loop.h"

seq_t a(idx_t n)
{
    if (n <= 5)
        return pow2_loop(n - 1);
    if (n == 6)
        return (seq_t)25;
    return pow2_loop(n - 2);
}
