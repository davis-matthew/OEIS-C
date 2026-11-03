/*
 * A193569  Unhappy numbers which enter the cycle (4, 16, 37, 58, 89, 145, 42,
 *          20) at 89.
 * https://oeis.org/A193569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer that is unhappy and first reaches
 *            the cycle (4,16,37,58,89,145,42,20) at 89
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Sum of the squares of the decimal digits of x. */
static seq_t sumsqdigits(seq_t x) {
    seq_t sum = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        seq_t d = tmp % 10;
        sum += d * d;
        tmp /= 10;
    }
    return sum;
}

/* Returns true if the number x is unhappy and its first occurrence
   of any cycle member is 89. */
static bool hit_89_first(seq_t x) {
    /* The eight numbers of the unhappy cycle. */
    static const seq_t cycle[8] = {4, 16, 37, 58, 89, 145, 42, 20};

    while (1) {
        /* If the process reaches 1, it is a happy number. */
        if (x == 1)
            return false;

        /* Check whether we have landed in the cycle. */
        bool in_cycle = false;
        for (int i = 0; i < 8; ++i) {
            if (x == cycle[i]) {
                in_cycle = true;
                return (x == 89);  /* qualifies only if 89 is first. */
            }
        }

        /* Continue iterating: x := sum of squares of its decimal digits. */
        x = sumsqdigits(x);
    }
}

/* Return the nth element of the sequence. */
seq_t a(idx_t n) {
    idx_t pos = 0;          /* count of qualifying numbers found so far */

    for (seq_t x = 1; ; ++x) {
        if (hit_89_first(x)) {
            ++pos;
            if (pos == n)
                return x;
        }
    }
}
