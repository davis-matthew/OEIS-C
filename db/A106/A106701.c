/*
 * A106701  a(n) = next-to-most-significant binary digit of n-th composite
 *          positive integer.
 * https://oeis.org/A106701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106701 - next-to-most-significant binary digit of the nth composite.
   Computed by iterating through integers, counting composites, and extracting
   the second‑most‑significant bit of the nth composite. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple composite test: works for 64‑bit integers and below.
   (All composites needed for n ≤ 10000 are far below 2³².) */
static bool is_composite_simple(uint64_t x)
{
    if (x < 4) return false;          /* 0,1,2,3 are not composite */
    if (x % 2 == 0) return true;      /* even numbers ≥4 are composite */
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return true;  /* found a divisor → composite */
    return false;                     /* no divisor found → prime */
}

/* Return the n‑th term (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* guard for out‑of‑range index */

    uint64_t candidate = 3;          /* candidates start at 4 */
    idx_t     count    = 0;           /* number of composites found */

    /* Find the n‑th composite. */
    while (count < n)
    {
        candidate++;                  /* next integer to test */
        if (is_composite_simple(candidate))
            count++;                  /* it's a composite */
    }

    seq_t composite = (seq_t)candidate; /* store the composite value */

    /* Determine position of the most significant bit (0‑based). */
    int msb_pos = 0;
    seq_t tmp = composite;
    while (tmp >> 1) {                 /* while there is a higher bit */
        msb_pos++;
        tmp >>= 1;
    }

    /* Extract the next‑to‑most‑significant bit.  For composites ≥4,
       msb_pos is at least 1, so msb_pos-1 is safe. */
    seq_t bit = (composite >> (msb_pos - 1)) & 1;
    return bit;
}
