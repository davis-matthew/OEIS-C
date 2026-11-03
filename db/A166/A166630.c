/*
 * A166630  Totally multiplicative sequence with a(p) = 9p for prime p.
 * https://oeis.org/A166630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * 9^Ω(n) where Ω(n) is the total number of prime
 *            factors of n counted with multiplicity. This uses the supplied
 *            auxiliary functions bigomega() and pow_uint()
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9707 (2026-11-01)
 * Helpers:   bigomega, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    /* The sequence is defined for n ≥ 1; for safety return 0 for n = 0. */
    if (n == 0) return 0;

    /* Ω(n) */
    seq_t omega = bigomega(n);
    /* Convert Ω(n) to idx_t for the exponent argument.  For n up to 10000
       this conversion is safe as Ω(n) < 15. */
    idx_t exp = (idx_t)omega;

    /* 9^Ω(n) */
    seq_t pow9 = ipow_u64_idx(9u, exp);

    /* a(n) = n * 9^Ω(n) */
    return (seq_t)n * pow9;
}
