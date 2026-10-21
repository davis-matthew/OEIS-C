/*
 * A054476  Numbers not divisible by any of their digits when written in base
 *          5.
 * https://oeis.org/A054476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   Numbers not divisible by any of their digits when written in
 *            base 5
 * Range:     u8 n <= 22, u16 n <= 4730, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t cand = 1;         /* candidate number in decimal */
    idx_t count = 0;        /* how many valid numbers found so far */

    for (;; ++cand)          /* iterate through all positive integers */
    {
        seq_t tmp = cand;
        bool ok = true;

        while (tmp != 0)    /* examine each base‑5 digit */
        {
            int d = (int)(tmp % 5);   /* current digit (0…4) */
            tmp = tmp / 5;

            if (d == 1) {           /* digit 1 divides every integer */
                ok = false;
                break;
            }
            if (d != 0 && cand % d == 0) {   /* divisibility test */
                ok = false;
                break;
            }
        }

        if (ok)                  /* valid number found */
        {
            ++count;
            if (count == n)
                return cand;
        }
    }
}
