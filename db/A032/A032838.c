/*
 * A032838  Numbers whose set of base-14 digits is {3,4}.
 * https://oeis.org/A032838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 510, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A032838 – numbers whose base‑14 digits are only 3 or 4.
   The nth term is obtained by:
   1. Find the length L of the base‑14 representation (L ≥ 1)
      such that 2^(L+1) – 2 ≥ n.
   2. Let offset = n – (2^L – 2) – 1  (0‑based within all numbers of length L).
   3. Interpret offset as an L‑bit binary number: bit 0 → digit 3, bit 1 → digit 4.
   4. Compute the value in base‑10 by
         val = Σ_{i=0}^{L-1} digit_{i} · 14^(L-1-i). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (out of range). */
    if (n == 0)
        return 0;

    /* Step 1: find L such that 2^(L+1) – 2 ≥ n. */
    idx_t limit = n + 2;          /* we need 2^(L+1) ≥ n+2 */
    idx_t pow   = 2;              /* pow = 2^(L+1) */
    idx_t L     = 0;              /* current length L (starting from 1) */

    while (pow < limit) {
        pow <<= 1;                /* multiply by 2 */
        ++L;
    }

    /* Step 2: compute the 0‑based offset within numbers of length L. */
    idx_t offset = n - (pow >> 1) + 1;   /* pow >> 1 == 2^L */

    /* Step 3 & 4: build the value digit by digit. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        val = val * 14;             /* shift left in base 14 */
        if (offset & ((idx_t)1 << (L - 1 - i)))
            val += 4;               /* digit 4 for bit 1 */
        else
            val += 3;               /* digit 3 for bit 0 */
    }

    return val;
}
