/*
 * A154721  Triangle read by rows in which row n lists 2n-1 terms: The pairs
 *          of noncomposite numbers equidistant to n, with 0's inserted, as
 *          shown below in the example.
 * https://oeis.org/A154721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = pos if both numbers at distance |pos - r| from the row
 *            index r are non‑composite (1 or prime); otherwise 0. Computes
 *            the linearized triangle A154721
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t idx)
{
    if (idx <= 1)
        return 0;

    /* Find the row r: smallest r such that r^2 >= idx */
    idx_t r = _floor_isqrt_u64(idx - 1) + 1;
    idx_t prev_sq = (r - 1) * (r - 1);
    idx_t pos = idx - prev_sq;          /* 1‑based position in the row */

    if (pos == r)                       /* center position */
        return 0;

    idx_t d = (pos < r) ? (r - pos) : (pos - r);

    idx_t left  = r - d;                /* number on the left side */
    idx_t right = r + d;                /* number on the right side */

    bool left_ok  = (left == 1) || is_prime(left);
    bool right_ok = (right == 1) || is_prime(right);

    return (left_ok && right_ok) ? (seq_t)pos : 0;
}
