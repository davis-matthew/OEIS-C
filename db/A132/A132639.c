/*
 * A132639  Catalan number C(n) raised to power C(n).
 * https://oeis.org/A132639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (C(n-1))^(C(n-1))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-01)
 * Helpers:   catalan, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t cat = catalan(n - 1);
    return ipow(cat, cat);
}
