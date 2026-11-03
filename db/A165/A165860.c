/*
 * A165860  Totally multiplicative sequence with a(p) = 39.
 * https://oeis.org/A165860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 39^bigomega(n) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 8825 (2026-11-01)
 * Helpers:   bigomega, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* For safety, if n==0 (outside the official offset) we return 1,
       since 39^0 = 1 and this keeps the function well‑defined. */
    if (n == 0)
        return ipow_u64_idx(39, 0);

    seq_t omega = bigomega(n);      /* total number of prime factors (with multiplicity) */
    return ipow_u64_idx(39, (idx_t)omega); /* 39^omega(n) */
}
