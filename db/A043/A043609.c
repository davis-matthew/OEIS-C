/*
 * A043609  Numbers whose base-5 representation has exactly 9 runs.
 * https://oeis.org/A043609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n), the n‑th integer whose base‑5 representation has exactly 9 runs. */
/* Algorithm: start from the smallest 9‑digit base‑5 number (5^8 = 390625) and
   count upwards, retaining numbers whose digit run count equals 9.
   The run count is determined by scanning the base‑5 digits of each candidate. */

#include <stdbool.h>

static inline int runs_in_base5(seq_t x)
{
    int runs = 0;
    int prev = -1;
    while (x > 0)
    {
        int d = (int)(x % 5);
        if (runs == 0 || d != prev)
            runs++;
        prev = d;
        x /= 5;
    }
    return runs;
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset 1, but behaviour for n==0 is irrelevant */
        return 0;

    const seq_t start = 390625; /* 5^8 */
    seq_t cur = start;
    idx_t found = 0;

    while (found < n)
    {
        if (runs_in_base5(cur) == 9)
        {
            found++;
            if (found == n)
                return cur;
        }
        cur++;
    }
    /* unreachable, keeps compiler happy */
    return 0;
}
