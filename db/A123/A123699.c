/*
 * A123699  Smallest b such that all digits are distinct in base b
 *          representation of n.
 * https://oeis.org/A123699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the smallest base b such that the representation of n in base b
 *              uses all distinct digits.
 *
 * The function simply tests bases from 2 up to n (or 1 for n==1),
 * converting each candidate base and checking for duplicate digits.
 * For n > 1 this works in O(n log n) time, which is trivial for n ≤ 10000.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: n == 1 is represented as "1" in base 1. */
    if (n == 1)
        return (seq_t) 1;

    /* Maximum number of digits when n fits in idx_t is 64 (base 2).
     * We use 128 for safety and to avoid variable‑length arrays. */
    enum { MAX_DIGITS = 128 };
    idx_t digits[MAX_DIGITS];

    for (idx_t base = 2; base <= n; ++base) {
        idx_t tmp = n;
        idx_t len = 0;

        /* Convert n to base 'base', storing digits in digits[0..len-1]. */
        while (tmp > 0) {
            digits[len++] = tmp % base;
            tmp /= base;
        }

        /* Check whether all digits are distinct. */
        bool distinct = true;
        for (idx_t i = 0; i < len && distinct; ++i)
            for (idx_t j = i + 1; j < len; ++j)
                if (digits[i] == digits[j]) {
                    distinct = false;
                    break;
                }

        if (distinct)
            return (seq_t) base;
    }

    /* Fallback: the representation in base n is always "10" (distinct). */
    return (seq_t) n;
}
