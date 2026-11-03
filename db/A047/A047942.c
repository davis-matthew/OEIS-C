/*
 * A047942  Number of 7-colorings of an n X n grid, up to rotational symmetry.
 * https://oeis.org/A047942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7^(n^2) + 2*7^((n^2+3*(n mod 2))/4) + 7^((n^2+(n mod
 *            2))/2))/4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    const seq_t m = 7;                    /* m = 7 */
    seq_t n_seq = (seq_t)n;               /* avoid integer overflow in multiplication */
    seq_t n2 = n_seq * n_seq;             /* n^2 */

    seq_t n_mod2 = (seq_t)(n & 1u);        /* n mod 2 */
    seq_t exp2 = (n2 + 3 * n_mod2) / 4;    /* (n^2 + 3*(n mod 2))/4 */
    seq_t exp3 = (n2 + n_mod2) / 2;        /* (n^2 + (n mod 2))/2 */

    seq_t term1 = ipow(m, n2);           /* 7^(n^2) */
    seq_t term2 = ipow(m, exp2);         /* 7^exp2 */
    seq_t term3 = ipow(m, exp3);         /* 7^exp3 */

    seq_t numerator = term1 + term2 * (seq_t)2 + term3;
    return numerator / (seq_t)4;
}
