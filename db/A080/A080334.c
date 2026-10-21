/*
 * A080334  n^2 read backwards, for n = 51, 50, 49, ..., 1.
 * https://oeis.org/A080334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = reverse_digits((52-n)^2) for 1 ≤ n ≤ 51, 0 otherwise
 * Range:     u8 none, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* reverse_digits removes leading zeros after reversal (e.g., 2500 → 52) */

#include <stddef.h>  /* for NULL, size_t */

seq_t a(idx_t n)
{
    /* n is 1‑based index. The sequence is defined for n = 1..51 only. */
    if (n > (idx_t)52)          /* n = 52 would give 0, but keep safe */
        return (seq_t)0;

    /* Compute k = 52 - n. Since n ≤ 52, the subtraction is safe. */
    idx_t k = (idx_t)52 - n;
    if (k == (idx_t)0)          /* n = 52 maps to 0 */
        return (seq_t)0;

    /* Compute square in seq_t to avoid overflow of intermediate 64‑bit */
    seq_t sq = (seq_t)k * (seq_t)k;

    /* Reverse digits of sq. */
    seq_t rev = 0;
    while (sq > (seq_t)0) {
        rev = rev * (seq_t)10 + sq % (seq_t)10;
        sq = sq / (seq_t)10;
    }
    return rev;
}
