/*
 * A044881  Numbers having, in base 10, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search for numbers with equal sum of lengths of runs of even
 *            length and odd length in their base‑10 representation
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Check whether a number satisfies the run‑length sum condition. */
static inline bool satisfies(seq_t x)
{
    if (x == 0) {
        return false;
    }
    seq_t odd_sum = 0;
    seq_t even_sum = 0;

    while (x > 0) {
        seq_t digit = x % 10;
        unsigned int len = 0;
        while (x > 0 && (x % 10) == digit) {
            x /= 10;
            len++;
        }
        if ((len & 1) == 0) {
            even_sum += len;
        } else {
            odd_sum += len;
        }
    }

    return odd_sum == even_sum;
}

/* a(n) returns the nth number (offset 1) satisfying the condition. */
seq_t a(idx_t n)
{
    seq_t cur = 0;
    idx_t count = 0;

    while (count < n) {
        if (cur > 0 && satisfies(cur)) {
            count++;
            if (count == n) {
                return cur;
            }
        }
        cur++;
    }
    /* The loop always returns, but the compiler may warn if it thinks we
       could reach here, so we return cur as a fallback. */
    return cur;
}
