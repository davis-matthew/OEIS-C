/*
 * A074492  Duplicate of A059307.
 * https://oeis.org/A074492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers: decimal representation uses only digits 2,3,5,6,7,8
   and the number is not a perfect square.                                 */
/* This function returns the nth such number (a(0)=2).                         */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t v = 2;                      /* start searching from 2          */
    for (;; ++v)
    {
        /* check all decimal digits */
        seq_t t = v;
        bool ok = true;
        while (t > 0)
        {
            int d = (int)(t % 10);    /* safely cast to small int.      */
            if (d == 0 || d == 1 || d == 4 || d == 9)
            {
                ok = false;
                break;
            }
            t /= 10;
        }

        if (!ok) continue;            /* digit rule failed              */

        /* check perfect‑square rule            */
        seq_t s = floor_sqrt(v);
        if (s * s == v) continue;     /* v is a square                   */

        if (cnt == n) return v;       /* found the desired term        */
        ++cnt;                        /* move to next candidate      */
    }
}
