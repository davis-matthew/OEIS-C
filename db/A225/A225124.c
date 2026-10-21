/*
 * A225124  a(n) = least number k such that 2^n is highest in Collatz(3x+1)
 *          trajectory of k.
 * https://oeis.org/A225124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225124:
   a(n) = least number k such that 2^n is the highest power of two
   occurring in the Collatz(3x+1) trajectory of k.
   Trajectory stops when reaching 1 (1 is a fixed point).
   The function below computes a(n) directly. */
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t cur  = pow2(n);   /* start from 2^n */
    seq_t best = cur;       /* minimal value found so far */

    while (1)
    {
        if ((cur & 1) == 0)                  /* cur is even */
        {
            seq_t cur_minus1 = cur - 1;
            /* check existence of odd predecessor: (cur-1)/3 */
            if ((cur_minus1 % 3) != 0)
                break;                      /* no valid odd predecessor */

            seq_t p = cur_minus1 / 3;        /* odd predecessor */
            if (p == 1)                     /* 1 is a fixed point, cannot lead to cur */
                break;

            if (p < best)
                best = p;                    /* update minimal value */

            cur = p << 1;                    /* next even node (double) */
        }
        else                                  /* cur is odd */
        {
            cur <<= 1;                        /* only possible move: double */
        }
    }

    return best;
}
