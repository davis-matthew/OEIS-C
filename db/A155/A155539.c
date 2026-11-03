/*
 * A155539  a(n) = n^(n+3) + (n+3)^n.
 * https://oeis.org/A155539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n+3) + (n+3)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 286 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* The big_pow function is provided in the utils library.
   It computes base^exp for arbitrary precision seq_t values. */
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t nseq   = (seq_t)n;
    seq_t nplus3 = nseq + (seq_t)3;

    /* Compute n^(n+3) and (n+3)^n */
    seq_t term1 = ipow(nseq,     (seq_t)(n + 3));
    seq_t term2 = ipow(nplus3, (seq_t)n);

    return term1 + term2;
}
