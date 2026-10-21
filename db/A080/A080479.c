/*
 * A080479  Smallest number formed by using all the digits (with multiplicity)
 *          of next n numbers.
 * https://oeis.org/A080479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the smallest integer that uses all digits (with
 *            multiplicity) of the successive n integers beginning at
 *            (n−1)*n/2 + 1. The digits are arranged in ascending order with
 *            the smallest non‑zero digit first (to avoid a leading zero).
 *            Example: n=4 → numbers 7,8,9,10 → digits {0,1,7,8,9} → 10789
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is O(n) and works for any n ≥ 1.  It uses only integer
   arithmetic on seq_t, so it satisfies both 64‑bit and arbitrary‑precision
   builds. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* start of the block of n consecutive integers */
    idx_t start = ((n - 1) * n) / 2 + 1;

    /* digit counts 0..9 */
    uint32_t cnt[10] = {0};

    /* Count digits of each integer in the block */
    for (idx_t i = 0; i < n; ++i) {
        idx_t value = start + i;
        /* every number is > 0 */
        while (value) {
            uint32_t dig = (uint32_t)(value % 10);
            cnt[dig]++;
            value /= 10;
        }
    }

    /* Find the smallest non‑zero digit to start the result */
    uint32_t first_digit = 0;
    for (uint32_t d = 1; d <= 9; ++d) {
        if (cnt[d] > 0) {
            first_digit = d;
            cnt[d]--;           /* consume one occurrence */
            break;
        }
    }

    if (first_digit == 0) {
        /* Should never happen, but just in case: all digits are zero */
        /* The block would then be empty, so return zero. */
        return (seq_t)0;
    }

    seq_t result = (seq_t)first_digit;

    /* Append all zero digits next (if any) */
    for (uint32_t i = 0; i < cnt[0]; ++i) {
        result = result * (seq_t)10 + (seq_t)0;
    }

    /* Append remaining digits in ascending order */
    for (uint32_t d = 1; d <= 9; ++d) {
        for (uint32_t i = 0; i < cnt[d]; ++i) {
            result = result * (seq_t)10 + (seq_t)d;
        }
    }

    return result;
}
