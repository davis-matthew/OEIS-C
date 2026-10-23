/*
 * A044939  Number of runs of even length in the base-8 representation of n.
 * https://oeis.org/A044939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs of even length in the base-8
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* offset starts at 1, behaviour for n < 1 is irrelevant */

    /* Collect base‑8 digits in reverse order */
    unsigned char digits[64];
    size_t len = 0;
    idx_t x = n;
    while (x > 0) {
        digits[len++] = (unsigned char)(x % (idx_t)8);
        x /= (idx_t)8;
    }

    seq_t count = 0;
    unsigned run_len = 0;
    unsigned prev_digit = 0;
    int has_prev = 0;              /* flag to indicate that a run has started */

    for (size_t i = 0; i < len; ++i) {
        unsigned d = digits[len - 1 - i];   /* most significant digit first */

        if (has_prev && d == prev_digit) {
            run_len++;
        } else {
            if (has_prev && (run_len & 1) == 0)
                count++;
            prev_digit = d;
            run_len = 1;
            has_prev = 1;
        }
    }

    if (has_prev && (run_len & 1) == 0)
        count++;

    return count;
}
