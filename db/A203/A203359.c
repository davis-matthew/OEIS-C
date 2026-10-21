/*
 * A203359  Number of nX5 binary arrays with every 1 immediately preceded by 0
 *          to the left or above.
 * https://oeis.org/A203359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 8*a(n-1) +148*a(n-2) +471*a(n-3) +578*a(n-4) *
 *            -25*a(n-5) -302*a(n-6) +171*a(n-7) +90*a(n-8) for n >= 9 *
 *            Initial values: * a(1)=8, a(2)=182, a(3)=3042, a(4)=55517,
 *            a(5)=984703, * a(6)=17631496, a(7)=314688054, a(8)=5622720187
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not part of the sequence, safe fallback */

    static seq_t init[9] = {
        0ULL,                            /* dummy for index 0 */
        8ULL,                            /* a(1) */
        182ULL,                          /* a(2) */
        3042ULL,                         /* a(3) */
        55517ULL,                        /* a(4) */
        984703ULL,                       /* a(5) */
        17631496ULL,                     /* a(6) */
        314688054ULL,                    /* a(7) */
        5622720187ULL                    /* a(8) */
    };

    if (n <= 8) return init[n];

    /* variables holding the eight preceding terms:
       r1 = a(i-1), r2 = a(i-2), ..., r8 = a(i-8)   */
    seq_t r1 = init[8];
    seq_t r2 = init[7];
    seq_t r3 = init[6];
    seq_t r4 = init[5];
    seq_t r5 = init[4];
    seq_t r6 = init[3];
    seq_t r7 = init[2];
    seq_t r8 = init[1];

    for (idx_t i = 9; i <= n; ++i) {
        /* compute the new term using the recurrence */
        seq_t sum_pos = ((seq_t)8  * r1) + ((seq_t)148 * r2) +
                        ((seq_t)471 * r3) + ((seq_t)578 * r4) +
                        ((seq_t)171 * r7) + ((seq_t)90  * r8);

        seq_t sum_neg = ((seq_t)25  * r5) + ((seq_t)302 * r6);

        seq_t new_term = sum_pos - sum_neg;

        /* shift the window for the next iteration */
        r8 = r7;
        r7 = r6;
        r6 = r5;
        r5 = r4;
        r4 = r3;
        r3 = r2;
        r2 = r1;
        r1 = new_term;
    }

    return r1;   /* r1 now holds a(n) */
}
