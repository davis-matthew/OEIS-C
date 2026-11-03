/*
 * A082253  Concatenation of (5n-4), (5n-3), (5n-2), (5n-1) and 5n divided by
 *          5.
 * https://oeis.org/A082253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(5n-4,5n-3,5n-2,5n-1,5n) / 5
 * Range:     u8 none, u16 n <= 1, u32 n <= 19, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 957 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t start = 5U * n - 4U;
    unsigned __int128 tmp = 0;
    for (idx_t i = 0; i < 5U; ++i)
    {
        idx_t part   = start + i;
        seq_t mult   = pow10((idx_t)num_digits((seq_t)part));
        tmp = tmp * (unsigned __int128)mult + (unsigned __int128)part;
    }
    tmp /= 5U;
    return (seq_t)tmp;
}
