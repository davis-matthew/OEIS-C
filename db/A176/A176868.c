/*
 * A176868  Greatest odd number that requires n Collatz (3x+1) iterations to
 *          reach 1, or zero if there is no such number.
 * https://oeis.org/A176868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176868: greatest odd number requiring n Collatz steps (including halving steps),
   or zero if no such odd number exists.
   Recurrence:
     a(0) = 1
     a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 0, a(6) = 0, a(8) = 0, a(10) = 0
     a(5) = 5
     For odd n ≥ 7:  a(n) = 4*a(n-2) + 1
     For even n ≥ 12: let k = n/2.  r = 1 if k%3==0, r = 5 if k%3==1, r = 21 if k%3==2.
                       a(n) = (4*a(n-3) - r) / 3
*/
#include <stdbool.h>

seq_t a(idx_t n)
{
    // array to hold the last four computed values
    seq_t buf[4] = {0};

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t val;

        if (i == 0)              val = 1;
        else if (i == 1 || i == 2 || i == 3 || i == 4 || i == 6 || i == 8 || i == 10)
                                val = 0;
        else if (i == 5)         val = 5;
        else if ((i & 1) == 0)   // even n ≥ 12
        {
            idx_t k = i >> 1;          // n/2
            int r = (k % 3 == 0) ? 1
                 : (k % 3 == 1) ? 5 : 21;
            seq_t temp = 4 * buf[(i - 3) & 3];
            val = (temp - (seq_t)r) / (seq_t)3;
        }
        else                      // odd n ≥ 7
        {
            seq_t temp = 4 * buf[(i - 2) & 3];
            val = temp + (seq_t)1;
        }

        buf[i & 3] = val;
    }

    return buf[n & 3];
}
