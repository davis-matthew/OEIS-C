/*
 * A159727  Number of permutations of 4 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 1 local maximum.
 * https://oeis.org/A159727
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*5^(n-2) for n>=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 88 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Values for n < 2 are irrelevant to the OEIS offset */
    if (n < 2)
        return seq_from_str("0");

    /* Compute 5^(n-2) */
    seq_t pow5 = ipow_idx(seq_from_str("5"), n - 2);

    /* Compute the multiplier 4*n as a seq_t */
    char buf[32];
    snprintf(buf, sizeof buf, "%" PRIu64, (uint64_t)n * 4U);
    seq_t mult = seq_from_str(buf);

    /* Return 4*n*5^(n-2) */
    return seq_mul(pow5, mult);
}
