/*
 * A116055  Triangular numbers whose representation is made of nontrivial runs
 *          of identical digits.
 * https://oeis.org/A116055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for triangular numbers whose decimal representation is made of
 * nontrivial runs of identical digits (each run length ≥ 2). */

#include <stdbool.h>

/* Return true iff the decimal representation of x consists entirely of runs
 * of identical digits, each run having length at least 2. */
static bool runs_valid(seq_t x)
{
    if (x == 0) return false;               /* triangular numbers start at 1 */
    seq_t prev = x % 10;
    x /= 10;
    unsigned run_len = 1;

    while (x > 0)
    {
        seq_t d = x % 10;
        x /= 10;
        if (d == prev)
        {
            run_len++;
        }
        else
        {
            if (run_len < 2) return false;
            prev = d;
            run_len = 1;
        }
    }
    return (run_len >= 2);
}

seq_t a(idx_t n)
{
    seq_t t = 0;           /* current triangular number T(k) */
    idx_t found = 0;       /* count of found terms */

    for (idx_t k = 1; ; ++k)
    {
        t += (seq_t)k;                 /* T(k) = T(k-1) + k */
        if (runs_valid(t))
        {
            ++found;
            if (found == n) return t;
        }
    }
}
