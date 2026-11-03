/*
 * A056069  Number of 4-element ordered antichains on an unlabeled n-element
 *          set; T_1-hypergraphs with 4 labeled nodes and n hyperedges.
 * https://oeis.org/A056069
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n+15,15) - 12*C(n+11,11) + 24*C(n+9,9) + 4*C(n+8,8) * -
 *            18*C(n+7,7) + 6*C(n+6,6) - 36*C(n+5,5) + 36*C(n+4,4) * +
 *            11*C(n+3,3) - 22*C(n+2,2) + 6*C(n+1,1) * for n ≥ 4
 * Range:     u8 none, u16 n <= 7, u32 n <= 20, u64 n <= 115, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 976 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* values below the offset are undefined */

    /* use signed 128‑bit intermediate for the sum so that
       cancellation between large terms works correctly. */
    signed __int128 sum = 0;

    sum +=  (signed __int128)binomial(n + 15, 15) *  1;
    sum +=  (signed __int128)binomial(n + 11, 11) * -12;
    sum +=  (signed __int128)binomial(n + 9,   9) *  24;
    sum +=  (signed __int128)binomial(n + 8,   8) *   4;
    sum +=  (signed __int128)binomial(n + 7,   7) * -18;
    sum +=  (signed __int128)binomial(n + 6,   6) *   6;
    sum +=  (signed __int128)binomial(n + 5,   5) * -36;
    sum +=  (signed __int128)binomial(n + 4,   4) *  36;
    sum +=  (signed __int128)binomial(n + 3,   3) *  11;
    sum +=  (signed __int128)binomial(n + 2,   2) * -22;
    sum +=  (signed __int128)binomial(n + 1,   1) *   6;

    return (seq_t)sum;
}
