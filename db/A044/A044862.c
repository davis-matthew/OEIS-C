/*
 * A044862  Positive integers having the same number of base-6 runs of odd
 *          length as even.
 * https://oeis.org/A044862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: returns the n-th positive integer whose base‑6 runs of odd
 * and even lengths are equal.  The function counts qualifying numbers
 * from 1 upwards until the n‑th is reached.  This direct test is
 * fast enough for n up to about 10,000. */
#include <stdbool.h>

static bool qualifies(seq_t x)
{
    if (x == 0)
        return false;

    seq_t tmp = x;
    unsigned char prev_digit = 255; /* sentinel for no previous digit */
    int run_len = 0;
    int odd_runs = 0, even_runs = 0;

    while (tmp > 0) {
        unsigned char digit = (unsigned char)(tmp % 6);
        tmp /= 6;
        if (digit == prev_digit) {
            run_len++;
        } else {
            if (prev_digit != 255) {
                if (run_len & 1)
                    odd_runs++;
                else
                    even_runs++;
            }
            prev_digit = digit;
            run_len = 1;
        }
    }
    /* process the final run */
    if (run_len & 1)
        odd_runs++;
    else
        even_runs++;

    return odd_runs == even_runs;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n < 1, but return 0 safely */

    seq_t candidate = 1;
    idx_t count = 0;

    for (;;) {
        if (qualifies(candidate)) {
            count++;
            if (count == n)
                return candidate;
        }
        candidate++;
    }
}
