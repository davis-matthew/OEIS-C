/*
 * A077268  Number of bases in which n requires at least one zero to be
 *          written.
 * https://oeis.org/A077268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = count of integers b with 2 ≤ b ≤ n such that the
 *            representation of n in base b contains at least one zero digit
 *            (not counting the leading digit)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9875 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_zero_in_base(idx_t n, idx_t base)
{
    idx_t m = n;
    while (m > 0) {
        idx_t rem = m % base;
        m /= base;
        if (rem == 0)
            if (m > 0)   /* zero not in the most‑significant digit */
                return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* no bases b ≥ 2 satisfy the condition */

    seq_t count = 0;
    for (idx_t b = 2; b <= n; ++b)
        if (has_zero_in_base(n, b))
            ++count;
    return count;
}
