/*
 * A043777  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 1 mod 4.
 * https://oeis.org/A043777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of A043777.  The sequence is the list of integers
   whose base‑3 representation has a number of runs congruent to 1
   modulo 4.  The function returns the n‑th such integer (a(1)=1,
   a(2)=2, …).  It is written to compile in both C11 and C++17,
   using only the operators allowed in the problem statement. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t val = 1;          /* current integer being tested      */
    idx_t  count = 0;       /* how many qualifying numbers found */

    while (count < n)
    {
        seq_t t = val;
        int  runs   = 0;    /* run counter for this number      */
        int  prev   = -1;   /* previous base‑3 digit            */

        /* Compute the number of runs in base 3. */
        while (t > 0)
        {
            int d = (int)(t % 3);
            if (d != prev)
            {
                runs++;
                prev = d;
            }
            t /= 3;
        }

        if (runs % 4 == 1)
        {
            count++;
            if (count == n)
                return val;
        }
        val++;
    }

    /* Control never reaches here for valid n>=1 */
    return 0;
}
