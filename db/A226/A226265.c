/*
 * A226265  Number of additive Z_2 Z_8 codes of a certain type (see
 *          Siap-Aydogdu for precise definition).
 * https://oeis.org/A226265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 168*(2^(n+1)-1)
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 100,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 98 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n == 0 return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute 2^(n+1).  Use the utility big_pow which works for both
       built‑in integer types and arbitrary‑precision types. */
    seq_t pow2 = ipow((seq_t)2, (seq_t)n + 1);

    /* a(n) = 168 * (2^(n+1) - 1) */
    return (pow2 - 1) * (seq_t)168;
}
