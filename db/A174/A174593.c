/*
 * A174593  a(n) is the smallest number that is larger by n than the sum of
 *          the square of its digits, or 0 if no such number exists.
 * https://oeis.org/A174593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest x > n such that x - sumsq(x) == n, otherwise 0.
 *            Computation: For any n, if a solution exists then x ≤ n +
 *            81*(digits(n)+1)
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Number of decimal digits of an idx_t value. */
static inline size_t digits_idx(idx_t v)
{
    size_t d = 0;
    do {
        ++d;
        v /= 10;
    } while (v);
    return d;
}

/* Sum of squares of decimal digits of a seq_t value. */
static inline seq_t digit_square_sum(seq_t v)
{
    seq_t sum = 0;
    while (v != 0) {
        seq_t d = v % 10;
        sum += d * d;
        v /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Compute maximum representable value in seq_t. */
    seq_t max_val = (seq_t)(~(seq_t)0);

    /* If n can't be represented in seq_t, there can be no valid x. */
    unsigned __int128 n128 = (unsigned __int128)n;
    if (n128 > (unsigned __int128)max_val) {
        return (seq_t)0;
    }

    /* Avoid overflow when n is already the maximum value. */
    if ((seq_t)n == max_val) {
        return (seq_t)0;
    }

    size_t d = digits_idx(n);
    unsigned __int128 ub128 = (unsigned __int128)n + (unsigned __int128)81 * (d + 1);
    seq_t upper = (seq_t)(ub128 > (unsigned __int128)max_val ? (unsigned __int128)max_val : ub128);

    seq_t start = (seq_t)((idx_t)n + 1);
    for (seq_t x = start; x <= upper; ++x) {
        if (x - digit_square_sum(x) == (seq_t)n) {
            return x;
        }
    }
    return (seq_t)0;
}
