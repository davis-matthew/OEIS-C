/*
 * A062183  Numbers such that harmonic mean of digits is 5.
 * https://oeis.org/A062183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  Brute‑force enumeration of integers.
   For each integer i, compute m = number of digits and
   S = Σ (5 / digit).  The condition harmonic mean = 5
   is equivalent to S = m, i.e. Σ (12600 / digit) = 2520 * m.
   Using pure integer arithmetic avoids floating‑point errors. */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t i = 1;
    idx_t   count = 0;

    for (;;) {
        uint64_t  x   = i;
        idx_t     m   = 0;

        /* Sum of 12600 / digit for the current number. */
        uint64_t  sum = 0;
        while (x != 0) {
            uint64_t d = x % 10;
            if (d == 0) {           /* digit 0 makes the term invalid */
                sum = UINT64_MAX;   /* a value that will never match */
                break;
            }
            sum += 12600ULL / d;    /* 5 * 2520 / d */
            x   /= 10;
            ++m;
        }

        if (sum == 2520ULL * m) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
        ++i;
    }
}
