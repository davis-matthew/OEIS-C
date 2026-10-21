/*
 * A226036  Let abc... be the decimal expansion of n. a(n) is the number of
 *          iterations of the map n -> f(n) needed to reach the last number of
 *          the cycle, where f(n) = a^a + b^b + c^c + ...
 * https://oeis.org/A226036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations of digit^digit sum (f) until a value
 *            repeats (cycle starts). a(0)=1, a(1)=0, a(354)=4, etc
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* 0^0 is defined as 1 */
static const seq_t POW_D[10] = {
    (seq_t)1,      /* 0^0 = 1 */
    (seq_t)1,      /* 1^1 = 1 */
    (seq_t)4,      /* 2^2 = 4 */
    (seq_t)27,     /* 3^3 = 27 */
    (seq_t)256,    /* 4^4 = 256 */
    (seq_t)3125,   /* 5^5 = 3125 */
    (seq_t)46656,  /* 6^6 = 46656 */
    (seq_t)823543, /* 7^7 = 823543 */
    (seq_t)16777216,   /* 8^8 = 16777216 */
    (seq_t)387420489    /* 9^9 = 387420489 */
};

#define MAX_VIS 8192   /* ample capacity for n ≤ 10000 */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;      /* current value of the iteration */
    seq_t steps = 0;           /* steps taken so far */

    static seq_t visited[MAX_VIS];
    size_t len = 0;          /* number of stored values */

    visited[len++] = cur;  /* store the starting value */

    for (;;)
    {
        /* compute f(cur) = Σ d^d over the decimal digits of cur */
        seq_t nxt = 0;
        seq_t tmp = cur;

        if (tmp == (seq_t)0)          /* special case: 0 has only digit 0 */
        {
            nxt = POW_D[0];
        }
        else
        {
            /* extract digits from least significant to most */
            while (tmp != (seq_t)0)
            {
                int d = (int)(tmp % (seq_t)10);  /* 0 ≤ d ≤ 9 */
                nxt += POW_D[d];
                tmp /= (seq_t)10;
            }
        }

        /* check whether nxt has appeared before */
        for (size_t i = 0; i < len; ++i)
        {
            if (visited[i] == nxt)
                return steps;
        }

        /* store nxt and continue */
        if (len >= MAX_VIS)          /* safety guard – should not happen */
            return steps;

        visited[len++] = nxt;
        cur = nxt;
        ++steps;
    }
}
