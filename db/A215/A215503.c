/*
 * A215503  a(n) = (u+1)^n + (-s-1)^n + (t+1)^n + (-1)^n + (-t+1)^n + (s-1)^n
 *          + (-u+1)^n where s = sqrt(2), t = sqrt(2-s), u = sqrt(2+s).
 * https://oeis.org/A215503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A215503
   a(n) = (u+1)^n + (-s-1)^n + (t+1)^n + (-1)^n + (-t+1)^n + (s-1)^n + (-u+1)^n
   where s = sqrt(2), t = sqrt(2-s), u = sqrt(2+s).
   Recurrence:
   a(0) = 7
   a(1) = 1
   a(2) = 19
   a(3) = 13
   a(4) = 111
   a(5) = 121
   a(6) = 763
   a(n) = a(n-1) + 9*a(n-2) - 5*a(n-3) - 17*a(n-4) + a(n-5) + 5*a(n-6) - a(n-7)
*/
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial seven terms */
    static const seq_t init[7] = {7, 1, 19, 13, 111, 121, 763};

    if (n < 7)
        return init[(size_t)n];

    /* buffer holds the most recent seven values: a(i-1), a(i-2), ..., a(i-7) */
    seq_t buf[7];
    for (size_t i = 0; i < 7; ++i)
        buf[6 - i] = init[i];        /* buf[0] = a6, buf[1] = a5, ..., buf[6] = a0 */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t x1 = buf[0];   /* a(i-1) */
        seq_t x2 = buf[1];   /* a(i-2) */
        seq_t x3 = buf[2];   /* a(i-3) */
        seq_t x4 = buf[3];   /* a(i-4) */
        seq_t x5 = buf[4];   /* a(i-5) */
        seq_t x6 = buf[5];   /* a(i-6) */
        seq_t x7 = buf[6];   /* a(i-7) */

        /* compute a(i) using the recurrence */
        seq_t res = x1;                /* a(i-1) */
        res += 9 * x2;                 /* + 9*a(i-2) */
        res -= 5 * x3;                 /* - 5*a(i-3) */
        res -= 17 * x4;                /* - 17*a(i-4) */
        res += x5;                     /* + a(i-5) */
        res += 5 * x6;                 /* + 5*a(i-6) */
        res -= x7;                     /* - a(i-7) */

        /* shift buffer for next iteration */
        buf[6] = buf[5];
        buf[5] = buf[4];
        buf[4] = buf[3];
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = res;
    }

    return buf[0];   /* now buf[0] holds a(n) */
}
