/*
 * A190620  Odd numbers with a single zero in their binary expansion.
 * https://oeis.org/A190620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 105, u32 n <= 465, u64 n <= 1953,
 *            u128 n <= 8001, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (2^m - 1) - 2^p.  
   m is the bit‑length (≥ 3) of the n‑th term; for each m there are m‑2
   terms.  The zero bit is at position p (0‑based from the LSB), where
   p = (m‑2) – offset and offset = n – (sum_{k=3}^{m‑1} (k‑2)) – 1. */

/* The pow2 function (in “pow2.h”) returns 2^exp as a seq_t. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the bit‑length m for which the n‑th term lies. */
    idx_t remaining = n;
    idx_t m = 3;                                /* minimal length (binary 101) */
    while (1) {
        idx_t group = m - 2;                     /* count of terms for this m */
        if (remaining <= group)
            break;
        remaining -= group;
        ++m;
    }

    /* Position of the zero bit within this m‑bit group. */
    idx_t offset = remaining - 1;               /* 0‑based within group */
    idx_t p = (m - 2) - offset;                 /* zero position (≥1) */

    /* Construct the value: all ones of length m minus 2^p. */
    seq_t all_ones = pow2(m);
    all_ones -= 1;                              /* 2^m – 1 */
    seq_t zero_mask = pow2(p);                   /* 2^p */
    return all_ones - zero_mask;
}
