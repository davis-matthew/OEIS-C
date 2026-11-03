/*
 * A055483  a(n) is the GCD of n and the reverse of n.
 * https://oeis.org/A055483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   a(n) = gcd(n, rev(n))
 * Range:     u8 n <= 104, u16 n <= 50000, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 127 terms through n = 9806 (2026-11-01)
 * Helpers:   gcd, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/reverse_digits_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t revn = (seq_t)reverse_digits_idx(n);
    return gcd((seq_t)n, revn);
}
