/*
 * A043670  Numbers whose base-15 representation has exactly 3 runs.
 * https://oeis.org/A043670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the nth integer whose base‑15 representation has exactly three runs
 * by brute‑force scanning.  The base‑15 representation of a positive
 * integer consists of its digits in base 15, without leading zeros.  An
 * integer has three runs if read from most‑significant to least‑significant
 * digit it changes value exactly twice.  For example 226 (base 15: 1 0 1)
 * has three runs, whereas 240 (base 15: 1 1 0) has only two.  The first
 * such integer is 226.  This routine produces a(n) for any n ≥ 1.
 */

#include <stdbool.h>

#define BASE 15ULL   /* base used for the representation */

/* Return true if val has exactly three runs in base 15.  Performs the
 * digit extraction from least‑significant to most‑significant digit,
 * counting changes in digit value.  Prunes early if more than three runs
 * are detected. */
static inline bool has_three_runs(seq_t val)
{
    int prev = -1;
    int runs = 0;
    while (val) {
        int digit = (int)(val % BASE);
        val /= BASE;
        if (digit != prev) {
            ++runs;
            prev = digit;
            if (runs > 3)
                return false;
        }
    }
    return runs == 3;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* not part of the offset; harmless */

    seq_t x = (seq_t)225;       /* first three‑digit base‑15 number (1 0 0) */
    seq_t count = 0;

    while (1) {
        if (has_three_runs(x)) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;                    /* next integer to test */
    }
}
