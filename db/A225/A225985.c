/*
 * A225985  List the positive numbers, remove even digits (including zeros)
 *          from each term; sequence = remaining terms.
 * https://oeis.org/A225985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth number obtained by
   taking the natural numbers that contain at least one odd decimal
   digit, deleting all even digits from each, and listing the
   resulting numbers in the same order. */

#include <stddef.h>

static inline int has_odd_digit(idx_t m)
{
    while (m) {
        if ((m % 10) & 1)
            return 1;
        m /= 10;
    }
    return 0;
}

static inline seq_t odd_part(idx_t m)
{
    /* extract digits into an array */
    int digits[20];
    size_t len = 0;
    while (m) {
        digits[len++] = m % 10;
        m /= 10;
    }
    /* build the result from most significant to least */
    seq_t res = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        int d = digits[i];
        if (d & 1) {
            res = res * 10 + d;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;
    while (count < n) {
        ++m;
        if (has_odd_digit(m)) {
            ++count;
            if (count == n)
                return odd_part(m);
        }
    }
    /* unreachable */
    return 0;
}
