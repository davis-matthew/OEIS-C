/*
 * A196221  a(n) = binomial(n+10, 10)*9^n.
 * https://oeis.org/A196221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+10,10)*9^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 359 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* b = C(n+10,10) */
    seq_t b = binomial(n + 10, 10);

    /* p = 9^n */
    seq_t p = ipow_idx((seq_t)9, n);

    /* a(n) = b * p; multiplication works for both builtin
       integer types and arbitrary‑precision classes. */
    return b * p;
}
