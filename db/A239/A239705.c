/*
 * A239705  Number of bases b for which the base-b alternate digital sum of n
 *          is -b.
 * https://oeis.org/A239705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula   : a(n) = number of bases b (2 ≤ b ≤ n) for which the
 *              alternate digital sum of n in base b equals –b.
 *           The alternate digital sum starts with a positive sign on
 *           the least significant digit and alternates signs on each
 *          higher digit:  (+d0) – d1 + d2 – d3 + …
 *           (base‑1 contributes 0 if n is even and 1 if n is odd,
 *            so it never satisfies the condition). */

#include <stdint.h>

static inline int64_t alt_digit_sum(idx_t n, idx_t base)
{
    int64_t sum = 0;
    int sign  = 1;          /* +1 for least significant digit */
    idx_t temp = n;

    while (temp > 0) {
        int digit = temp % base;   /* LSD first */
        sum += sign * digit;
        sign = -sign;
        temp /= base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    if (n < 2)          /* for n = 1 only base‑1 exists, but it never fits */
        return 0;

    /* bases larger than n give a single‑digit representation: sum = n
       which can never be –b (negative).  Hence we only need to check
       bases b = 2 … n. */
    for (idx_t b = 2; b <= n; ++b) {
        if (alt_digit_sum(n, b) == -(int64_t)b)
            ++count;
    }
    return count;
}
