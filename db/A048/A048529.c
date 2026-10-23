/*
 * A048529  a(n) is the smallest value of m such that A002378(m), the m-th
 *          oblong number, contains exactly n 0's.
 * https://oeis.org/A048529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048529:  a(n) = the smallest m such that the oblong number
   m·(m+1) contains exactly n zeros in its decimal representation. */

#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 0 (A002378(0) = 0 has one zero) */
    if (n == 1) return 0;

    /* Special small cases */
    if (n == 2) return 24;   /* 24·25 = 600 has two zeros */
    if (n == 4) return 624;  /* 624·625 = 390000 has four zeros */

    if (n & 1)  /* n odd:   zeros = 2·k – 1  →  k = (n+1)/2  →  m = 10^k */
    {
        idx_t k = (n + 1) / 2;
        return pow10(k);
    }
    /* n even and n ≥ 6: zeros = 2·k – 2  →  k = (n+2)/2  →  m = 10^k + 1 */
    {
        idx_t k = (n + 2) / 2;
        seq_t v = pow10(k);
        return v + 1;
    }
}
