/*
 * A140429  a(n) = floor(3^(n-1)).
 * https://oeis.org/A140429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3^(n-1))
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 969 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0)
    {
        return (seq_t)0;
    }
    return ipow((seq_t)3, (seq_t)(n - 1));
}
