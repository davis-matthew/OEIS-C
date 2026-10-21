/*
 * A239706  Number of bases b for which the absolute value of the base-b
 *          alternate digital sum of n is b.
 * https://oeis.org/A239706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of bases b (1 ≤ b ≤ n) such that |altDigitSum_b(n)| = b
 *            (OEIS A239706). The computation is a direct brute‑force
 *            evaluation of the alternate digit sum for each base, which is
 *            O(n log n) operations and easily fast enough for n up to 10 000
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   abs_alt_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */
#include "../utils/abs_alt_digit_sum.h"

/* Forward declaration for the helper that gives the absolute
   alternate digit sum in base b. */
static seq_t abs_alt_digit_sum(idx_t n, idx_t b);

/* Public sequence function */
seq_t a(idx_t n)
{
    /* The OEIS sequence is defined for n ≥ 1; for safety return 0 for n = 0. */
    if (n == 0)
        return 0;

    seq_t count = 0;

    for (idx_t b = 1; b <= n; ++b) {
        seq_t abs_sum = abs_alt_digit_sum(n, b);
        if (abs_sum == (seq_t)b)
            ++count;
    }
    return count;
}

/* ------------------------------------------------------------------ */
