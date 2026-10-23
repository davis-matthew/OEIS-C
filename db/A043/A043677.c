/*
 * A043677  Numbers whose base-16 representation has exactly 4 runs.
 * https://oeis.org/A043677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force enumeration of integers whose base‑16 representation has exactly 4 runs
   (OEIS A043677).  For each candidate integer, the hexadecimal digits are examined
   and the number of runs is counted; when the count reaches 4 the value is taken
   as a term of the sequence. */

#include <stdbool.h>

static bool has_four_runs(seq_t x)
{
    int prev_digit = -1;
    int runs = 0;
    seq_t y = x;
    while (y != 0) {
        /* Extract lowest 4 bits (a hexadecimal digit). */
        int digit = (int)(y & 15);
        if (digit != prev_digit) {
            ++runs;
            prev_digit = digit;
            if (runs > 4)  /* early exit if too many runs */
                return false;
        }
        y >>= 4;          /* divide by 16 */
    }
    return runs == 4;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val = 0;
    while (1) {
        if (has_four_runs(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
