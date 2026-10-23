/*
 * A039088  Numbers whose base-9 representation has the same number of 1's and
 *          6's.
 * https://oeis.org/A039088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A039088.
   Numbers whose base‑9 representation has the same number of 1's and 6's.
   a(1) = 0, a(2) = 2, a(3) = 3, ...
   The algorithm simply tests consecutive integers, counts the
   occurrences of digits 1 and 6 in base‑9, and returns the n‑th value
   matched.  No memoisation or dynamic allocation is required, making
   it valid both in C11 and in C++17 regardless of whether seq_t is
   a built‑in unsigned type or an arbitrary‑precision class. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t found = 0;                 /* how many valid numbers we have seen */
    seq_t candidate = (seq_t)0;      /* current integer to test */
    const seq_t zero   = (seq_t)0;
    const seq_t nine   = (seq_t)9;

    while (found < n) {
        seq_t x = candidate;
        int cnt1 = 0, cnt6 = 0;

        /* Count the number of 1's and 6's in the base‑9 expansion of x. */
        seq_t tmp = x;
        while (tmp > zero) {
            seq_t digit = tmp % nine;
            if (digit == (seq_t)1)
                ++cnt1;
            else if (digit == (seq_t)6)
                ++cnt6;
            tmp = tmp / nine;
        }

        if (cnt1 == cnt6) {
            ++found;
            if (found == n)
                return candidate;
        }

        ++candidate;
    }
    /* The loop always returns inside; this line is never reached. */
    return (seq_t)0;
}
