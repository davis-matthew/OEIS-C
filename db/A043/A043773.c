/*
 * A043773  Numbers k such that number of runs in the base 3 representation of
 *          k is congruent to 0 mod 3.
 * https://oeis.org/A043773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th integer k whose base‑3 representation has a number of runs
   congruent to 0 modulo 3.  The function scans integers from 1 upwards and
   counts the runs.  No memoisation or external helpers are required. */

#include <stdbool.h>

/* Count base‑3 runs of x.  Return true if runs ≡ 0 mod 3. */
static bool good_runs(seq_t x)
{
    int runs   = 0;
    int prev   = -1;          /* value of the previous base‑3 digit */
    seq_t t    = x;

    while (t > 0) {
        int d = (int)(t % 3);   /* current digit (0,1,2) */
        if (d != prev) {
            ++runs;
            prev = d;
        }
        t /= 3;
    }
    return (runs % 3) == 0;
}

/* Main sequence function.  It scans integers from 1 upwards until the
   n‑th qualifying number is found.  This runs in O(n) time with a
   negligible constant factor and works for any integer type seq_t
   (including arbitrary‑precision types when compiled as C++). */
seq_t a(idx_t n)
{
    idx_t  count = 0;    /* number of qualifying integers found so far */
    idx_t  k     = 1;    /* integer to test */

    while (count < n) {
        seq_t x = (seq_t)k;          /* cast to seq_t for the test */
        if (good_runs(x)) {
            ++count;
            if (count == n)
                return x;
        }
        ++k;
    }
    /* The loop always returns before reaching here; the statement satisfies
       the compiler's requirement for a return on all paths. */
    return 0;
}
