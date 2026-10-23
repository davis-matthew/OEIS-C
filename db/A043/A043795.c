/*
 * A043795  Numbers whose number of runs in base 3 is congruent to 4 (mod 7).
 * https://oeis.org/A043795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043795: Numbers whose number of runs in base 3 is congruent to 4 (mod 7).  
   Computation: iterate integers, count runs of base‑3 representation, keep those with
   run_count % 7 == 4. */
#include <stdbool.h>

static inline int runs_in_base3(seq_t x)
{
    if (x == (seq_t)0)
        return 1;                      /* base‑3 “0” has one run */
    int prev = -1;
    int runs = 0;
    while (x > (seq_t)0)
    {
        int digit = (int)(x % (seq_t)3);
        x = x / (seq_t)3;
        if (digit != prev)
        {
            ++runs;
            prev = digit;
        }
    }
    return runs;
}

seq_t a(idx_t n)
{
    idx_t target = n;
    idx_t count  = 0;
    seq_t val = 0;
    for (;; ++val)
    {
        int run = runs_in_base3(val);
        if (run % 7 == 4)
        {
            ++count;
            if (count == target)
                return val;
        }
    }
}
