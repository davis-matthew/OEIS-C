/*
 * A214667  The y-coordinates of prime numbers in an Ulam spiral oriented
 *          counterclockwise with first step west.
 * https://oeis.org/A214667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6572, i16 n <= 9473, i32 n <= 9473, i64 n <= 9473,
 *            i128 n <= 9473, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: the y–coordinate of the (n)th prime in an
   Ulam spiral that is turned counter‑clockwise and whose first
   step is west.  The spiral is constructed incrementally; when
   the counter reaches the n-th prime the current y–coordinate
   is returned. */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Return the y‑coordinate of the n‑th prime in the
       Ulam spiral described above. */
    if (n == 0) return 0;          /* index 0 is unused */

    idx_t count = 0;               /* number of primes seen so far */
    seq_t x = 0, y = 0;            /* current grid position */
    idx_t len = 1;                 /* current step length */
    int dir = 0;                   /* 0=W,1=S,2=E,3=N */

    idx_t k = 2;                   /* integer value being placed */
    for (;;)
    {
        for (int repeat = 0; repeat < 2; ++repeat)
        {
            for (idx_t step = 0; step < len; ++step, ++k)
            {
                /* move one step in current direction */
                switch (dir)
                {
                case 0: --x; break;   /* West */
                case 1: --y; break;   /* South */
                case 2: ++x; break;   /* East */
                case 3: ++y; break;   /* North */
                }

                if (is_prime(k))
                {
                    ++count;
                    if (count == n) return y;
                }
            }
            dir = (dir + 1) & 3;     /* turn left (counter‑clockwise) */
        }
        ++len;                       /* increase step length */
    }
}
