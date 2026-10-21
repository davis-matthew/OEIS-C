/*
 * A061479  Smallest number m such that first digit - second digit + third
 *          digit - fourth digit ... (of m) = n.
 * https://oeis.org/A061479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061479
   Smallest number m such that first digit – second digit + third digit – ...  = n.
   The number is always of the form
          d 0 9 0 9 … 0 9
   where d = n – 9·k, k = ⌊(n–1)/9⌋ (k ≥ 0, 1 ≤ d ≤ 9).  Thus
   a(n) = d followed by k copies of the string "09". */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 9)
        return (seq_t)n;          /* single digit numbers */

    idx_t k = (n - 1) / 9;          /* number of “09” pairs */
    int    d = (int)(n - 9 * k);    /* first (non‑zero) digit, 1…9 */

    seq_t result = (seq_t)d;        /* start with the first digit */

    for (idx_t i = 0; i < k; ++i) {
        result = result * 10 + 0;   /* append 0 */
        result = result * 10 + 9;   /* append 9 */
    }

    return result;
}
