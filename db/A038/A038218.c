/*
 * A038218  Triangle whose (i,j)-th entry is binomial(i,j)*2^(i-j)*12^j (with
 *          i, j >= 0).
 * https://oeis.org/A038218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = binomial(i,j) * 2^(i-j) * 12^j where i satisfies
 *            i*(i+1)/2 ≤ n < (i+1)*(i+2)/2 and j = n - i*(i+1)/2
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Find the row i by iterating until the next triangular number exceeds n. */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;

    /* Column index j */
    idx_t j = n - i * (i + 1) / 2;

    /* Compute the triangle entry T(i,j) = C(i,j) * 2^(i-j) * 12^j. */
    seq_t bin = binomial(i, j);
    seq_t p2  = ipow_u64_idx(2u, i - j);   /* 2^(i-j) */
    seq_t p12 = ipow_u64_idx(12u, j);      /* 12^j */

    seq_t result = bin;
    result *= p2;
    result *= p12;

    return result;
}
