/*
 * A043642  Numbers whose base-10 representation has exactly 6 runs.
 * https://oeis.org/A043642
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

/* Sequence A043642: Numbers whose base‑10 representation has exactly 6 runs.
   The routine computes a(n) by brute‑force enumeration.  For each integer
   starting at the first 6‑digit number, it counts the number of runs in
   its decimal digits and returns the nth one found. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; n<1 is undefined, but we sane‑return 0 */
        return 0;

    seq_t found = 0;          /* how many terms seen so far     */
    seq_t num   = (seq_t)100000;  /* start from the first 6‑digit number */

    while (1)
    {
        /* Count runs in the decimal representation of num */
        seq_t t   = num;
        int    prev = -1;  /* sentinel for “no previous digit” */
        seq_t  runs = 0;

        while (t > 0)
        {
            int d = (int)(t % 10);
            if (d != prev)
            {
                runs++;
                prev = d;
            }
            t /= 10;
        }

        if (runs == 6)
        {
            found++;
            if (found == n)
                return num;   /* found the desired term */
        }

        ++num;   /* continue searching */
    }
}
