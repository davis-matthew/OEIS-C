/*
 * A160746  a(n) = A151566(n)*4.
 * https://oeis.org/A160746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * A151566(n) A151566(0) = 0 A151566(1) = 1 A151566(2) =
 *            2 For n ≥ 2: if n is a power of two (n = 2 k, k ≥ 2) A151566(n)
 *            = 3 · A151566(n/2) else let m = 2⌊log₂ n⌋ (the largest power of
 *            two < n) and r = n – m A151566(n) = A151566(m) + 2 · A151566(r)
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A151566(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* find the greatest power of two ≤ n */
    idx_t m = 1;
    while (m <= n >> 1) m <<= 1;

    if (m == n) /* n is a power of two, n ≥ 4 */
        return (seq_t)3 * A151566(n >> 1);

    /* n = m + r, where r = n - m < m */
    idx_t r = n - m;
    return A151566(m) + (seq_t)2 * A151566(r);
}

seq_t a(idx_t n)
{
    return (seq_t)4 * A151566(n);
}
