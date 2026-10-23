/*
 * A173022  Number of numbers <= n whose binary representation is without
 *          isolated ones.
 * https://oeis.org/A173022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1774, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173022: Number of numbers <= n whose binary representation is without isolated ones.
   This implementation counts all numbers 0 … n inclusive by testing each integer
   whether every run of consecutive 1’s has length at least 2. 0 is counted as it
   has no isolated 1’s. */

#include <stdbool.h>
#include <stdint.h>

static inline bool no_isolated(idx_t x)
{
    /* 0 has no isolated 1’s */
    if (x == 0) return true;

    int run = 0;            /* current run length of consecutive 1’s */
    for (idx_t i = 0; x >> i; ++i)
    {
        int bit = (int)((x >> i) & 1);
        if (bit)
        {
            run++;
        }
        else
        {
            if (run == 1) return false; /* isolated 1 found */
            run = 0;
        }
    }
    /* check last run after the loop */
    if (run == 1) return false;
    return true;
}

extern seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        if (no_isolated(i))
            cnt++;
    }
    return cnt;
}
