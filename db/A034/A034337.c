/*
 * A034337  Number of inequivalent binary [ n,3 ] codes of dimension <= 3
 *          without zero columns.
 * https://oeis.org/A034337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A034337
 *
 * A(n+21) =  2*A(n+20) +0*A(n+19)+0*A(n+18)-2*A(n+17)-2*A(n+16)+3*A(n+15)+
 *            1*A(n+14)+1*A(n+13)-2*A(n+12)-2*A(n+11)+2*A(n+10)+2*A(n+9)-
 *            1*A(n+8) -1*A(n+7) -3*A(n+6)+2*A(n+5)+2*A(n+4)+0*A(n+3)+0*A(n+2)-
 *           2*A(n+1)+1*A(n)
 *
 * Initial terms (n = 1 … 21):
 * 1, 2, 4, 7, 11, 19, 29, 44, 66, 96,
 * 136, 193, 265, 361, 485, 643, 841, 1093, 1401, 1782, 2248
 *
 * The code below implements this linear recurrence in O(n) time.
 * Only standard arithmetic operators are used, avoiding any
 * floating‑point or large‑integer libraries.
 */

#include <stdint.h>

static const int rec[21] = {
    2,  0,  0, -2, -2,  3,  1,  1, -2, -2,
    2,  2, -1, -1, -3,  2,  2,  0,  0, -2,  1
};

static const seq_t init[21] = {
    1,  2,  4,  7, 11, 19, 29, 44, 66, 96,
   136, 193, 265, 361, 485, 643, 841,1093,1401,1782,2248
};

seq_t a(idx_t n)
{
    if (n <= 21) return init[n - 1];

    /* buf holds the last 21 values a(i-20) … a(i) */
    seq_t buf[21];
    for (int i = 0; i < 21; ++i) buf[i] = init[i];

    for (idx_t i = 22; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        /* recompute next term using the recurrence */
        for (int j = 0; j < 21; ++j) {
            const int c = rec[j];
            if (c > 0)
                pos += buf[20 - j] * (seq_t)c;
            else if (c < 0)
                neg += buf[20 - j] * (seq_t)(-c);
        }
        seq_t nxt = pos - neg;

        /* shift buffer left, drop the oldest, insert newest */
        for (int j = 0; j < 20; ++j) buf[j] = buf[j + 1];
        buf[20] = nxt;
    }

    return buf[20];
}
