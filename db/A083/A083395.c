/*
 * A083395  Multiply the digits in pairs, stop when reach a number with an odd
 *          number of digits.
 * https://oeis.org/A083395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = transform^(n-1)(986888) where transform(x) = concatenate
 *            the products of each non‑overlapping adjacent decimal digit pair
 *            of x
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   digit_len, transform
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_len.h"
#include "../utils/transform.h"

seq_t a(idx_t n)
{
    const seq_t seed = 986888;
    if (n <= 1)
        return seed;

    seq_t cur = seed;
    for (idx_t i = 2; i <= n; ++i) {
        /* stop if the current number already has an odd number of digits */
        if (digit_len(cur) % 2 == 1)
            break;
        cur = transform(cur);
    }
    return cur;
}
