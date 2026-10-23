/*
 * A043008  Base-5 palindromes that start with 3.
 * https://oeis.org/A043008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A043008
 *  Base-5 palindromes that start with 3, enumerated in increasing order.
 *  This function computes the nth term directly by generating the
 *  corresponding base‑5 palindrome.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case: single digit 3 */
    if (n == 1)
        return (seq_t)3;

    /* Precompute powers of 5 up to 5^20 (more than enough for n <= 10000) */
    static bool init = false;
    static idx_t pow5[21];
    if (!init) {
        pow5[0] = 1;
        for (int i = 1; i < 21; ++i) {
            pow5[i] = pow5[i-1] * (idx_t)5;
        }
        init = true;
    }

    idx_t len = 1;        /* current length of palindrome */
    idx_t remaining = n;  /* terms remaining to reach the desired index */

    /* Find length such that cumulative count exceeds or equals n */
    while (true) {
        idx_t count; /* number of palindromes of this length */

        if (len == 1) {
            count = 1;
        } else {
            /* t = number of independent internal digits */
            idx_t t = (len - 1) / 2;
            count = pow5[(size_t)t];
        }

        if (remaining <= count)
            break;

        remaining -= count;
        ++len;
    }

    /* remaining is the 1‑based index within palindromes of length 'len' */
    idx_t rank = remaining - 1;          /* convert to 0‑based */
    idx_t t = (len - 1) / 2;             /* number of internal digit pairs */
    int digits[32] = {0};                /* sufficiently large buffer */

    /* Leading and trailing digit */
    digits[0] = digits[len - 1] = 3;

    /* Extract digits from base‑5 representation of 'rank' */
    for (idx_t ii = t; ii > 0; --ii) {
        int d = (int)(rank % 5);
        digits[ii] = d;
        digits[len - 1 - ii] = d;
        rank /= 5;
    }

    /* Build the numeric value from digits */
    seq_t val = 0;
    for (idx_t i = 0; i < len; ++i) {
        val = val * (seq_t)5 + (seq_t)digits[i];
    }

    return val;
}
