/*
 * A196741  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,3,1,0,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A196741
   Formula: a(n) = 2*a(n-1) + 5*a(n-2) + 12*a(n-3) + 2*a(n-4)
             - 11*a(n-5) + 4*a(n-6) - 5*a(n-7) + a(n-8)
   Initial conditions: a(1)=1, a(2)=3, a(3)=20, a(4)=71,
                      a(5)=282, a(6)=1144, a(7)=4559, a(8)=18153 */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*  pre‑computed first eight terms, 0‑based */
    static const seq_t init[8] = {
        (seq_t)1,
        (seq_t)3,
        (seq_t)20,
        (seq_t)71,
        (seq_t)282,
        (seq_t)1144,
        (seq_t)4559,
        (seq_t)18153
    };

    if (n <= 8) {
        return init[n - 1];
    }

    /* last[0] = a(n-1), last[1] = a(n-2), ..., last[7] = a(n-8) */
    seq_t last[8];
    last[0] = init[7];  /* a8 */
    last[1] = init[6];  /* a7 */
    last[2] = init[5];  /* a6 */
    last[3] = init[4];  /* a5 */
    last[4] = init[3];  /* a4 */
    last[5] = init[2];  /* a3 */
    last[6] = init[1];  /* a2 */
    last[7] = init[0];  /* a1 */

    for (idx_t i = 9; i <= n; ++i) {
        /* positive part */
        seq_t pos = 2 * last[0] + 5 * last[1] + 12 * last[2]
                    + 2 * last[3] + 4 * last[5] + last[7];
        /* negative part */
        seq_t neg = 11 * last[4] + 5 * last[6];
        seq_t term = pos - neg;           /* safe: pos >= neg */

        /* shift the window */
        last[7] = last[6];
        last[6] = last[5];
        last[5] = last[4];
        last[4] = last[3];
        last[3] = last[2];
        last[2] = last[1];
        last[1] = last[0];
        last[0] = term;
    }

    return last[0];
}
