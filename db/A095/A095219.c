/*
 * A095219  First differences of A053067.
 * https://oeis.org/A095219
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, less, nonn
 * Formula:   a(n) = concat_tri(n) - concat_tri(n-1) * where concat_tri(m) =
 *            concatenation of all integers from m(m-1)/2+1 * to m(m+1)/2,
 *            inclusive. For n ≥ 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 15 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t concat_tri(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t res = 0;
    /* Compute start = n(n-1)/2 + 1, end = n(n+1)/2   (all 64‑bit safe for n up to ~10¹⁶) */
    uint64_t start = (uint64_t)(( (uint64_t)(n - 1) * n ) / 2) + 1;
    uint64_t end   = (uint64_t)(( (uint64_t)n * (n + 1) ) / 2);

    for (uint64_t k = start; k <= end; ++k)
    {
        uint64_t digs = num_digits0(k);
        seq_t pow = pow10(digs);
        res = res * pow + (seq_t)k;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* undefined for n < 2 */
    return concat_tri(n) - concat_tri(n - 1);
}
