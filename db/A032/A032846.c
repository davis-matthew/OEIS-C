/*
 * A032846  Numbers whose base-8 representation Sum_{i=0..m} d(i)*8^i has d(m)
 *          < d(m-1) > d(m-2) < ...
 * https://oeis.org/A032846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th natural number whose base‑8 digits satisfy d(m)
 *            < d(m‑1) > d(m‑2) < … (starting from the most significant digit)
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alt_octal_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alt_octal_ok.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val = 0;

    while (count < n) {
        ++val;                                 /* next integer to test */
        if (alt_octal_ok(val))
            ++count;
    }

    return val;
}
