/*
 * A166726  Nonnegative integers with English names ending in "o".
 * https://oeis.org/A166726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(1) = 0; for n ≥ 2, let k = n – 1. Let full_blocks = (k – 1) /
 *            9, let r = k – full_blocks * 9 (1 ≤ r ≤ 9) offset = (r == 1) ? 0
 *            : r m = full_blocks * 10 + offset a(n) = 10 * m + 2 This yields
 *            all non‑negative integers whose English name ends in “o”. (Zero)
 *            ∪ {numbers ending in “two” but not “twe...
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* everything after zero is counted starting at k = n-1 */
    seq_t k = (seq_t)(n - 1);          /* k ≥ 1 */
    seq_t full_blocks = (k - 1) / 9;   /* 0‑based block index */
    seq_t r = k - full_blocks * 9;     /* 1…9 */
    seq_t offset = (r == 1) ? (seq_t)0 : r;
    seq_t m = full_blocks * 10 + offset;
    return m * 10 + 2;
}
