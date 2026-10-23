/*
 * A033018  Numbers whose base-5 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 375, u32 n <= 1400, u64 n <= 1400,
 *            u128 n <= 1400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A033018: Numbers whose base‑5 expansion has no run of length 1.
   Implementation by brute‑force search.  For a given n (n ≥ 1) it
   scans integers in increasing order, counting numbers that satisfy
   the property until the nth such number is found.  No caching or
   dynamic memory is used, so the code is portable to both C11 and
   C++17, regardless of the concrete definition of seq_t. */

#include <stdbool.h>

/* Return true if x has no run of a single digit in base‑5. */
static bool valid_base5(seq_t x)
{
    if (x == 0)                /* single digit “0” is invalid */
        return false;

    seq_t tmp = x;
    unsigned prev_digit = (unsigned)(tmp % 5);
    unsigned run_len    = 1;
    tmp /= 5;

    while (tmp > 0) {
        unsigned cur_digit = (unsigned)(tmp % 5);
        if (cur_digit == prev_digit) {
            ++run_len;
        } else {
            if (run_len < 2)      /* run of length 1 found */
                return false;
            prev_digit = cur_digit;
            run_len    = 1;
        }
        tmp /= 5;
    }
    /* Check the final run */
    return run_len >= 2;
}

/* Return the nth term of sequence A033018 (offset 1). */
seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t count = 0;

    while (count < n) {
        if (valid_base5(candidate))
            ++count;
        ++candidate;
    }
    /* candidate has been incremented once past the desired value */
    return candidate - 1;
}
