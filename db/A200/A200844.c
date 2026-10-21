/*
 * A200844  Number of 0..n arrays x(0..8) of 9 elements without any two
 *          consecutive increases or two consecutive decreases.
 * https://oeis.org/A200844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) +
 *            252*a(n-5) * - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) -
 *            a(n-10) * Initial values (n=1..10): * a(1)=512, a(2)=11235,
 *            a(3)=99700, a(4)=552247, a(5)=2276028, * a(6)=7642875,
 *            a(7)=22063924, a(8)=56687801, a(9)=132734964, * a(10)=288116455
 * Range:     u8 none, u16 n <= 2, u32 n <= 14, u64 n <= 193, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of the first 10 terms – they are needed for all
     * further calculations and for base cases.
     */
    static const seq_t base[10] = {
        512ULL,        /* a(1) */
        11235ULL,      /* a(2) */
        99700ULL,      /* a(3) */
        552247ULL,     /* a(4) */
        2276028ULL,    /* a(5) */
        7642875ULL,    /* a(6) */
        22063924ULL,   /* a(7) */
        56687801ULL,   /* a(8) */
        132734964ULL,  /* a(9) */
        288116455ULL   /* a(10) */
    };

    if (n <= 10)                 /* direct answer for the base cases */
        return base[n - 1];

    /* sliding window of the last ten values, initially set to a(1)..a(10) */
    seq_t window[10];
    for (int i = 0; i < 10; ++i)
        window[i] = base[i];

    /* compute a(11) .. a(n) iteratively */
    for (idx_t cur = 11; cur <= n; ++cur) {
        /* compute the recurrence using only unsigned arithmetic:
         *   positive  terms: 10,120,252,120,10
         *   negative  terms: 45,210,210,45,1
         */
        seq_t pos = window[9] * 10ULL;                    /* a(n-1)   *10   */
        pos += window[7] * 120ULL;                       /* a(n-3)  *120  */
        pos += window[5] * 252ULL;                       /* a(n-5)  *252  */
        pos += window[3] * 120ULL;                       /* a(n-7)  *120  */
        pos += window[1] * 10ULL;                        /* a(n-9)   *10  */

        seq_t neg = window[8] * 45ULL;                    /* a(n-2)  *45   */
        neg += window[6] * 210ULL;                       /* a(n-4) *210   */
        neg += window[4] * 210ULL;                       /* a(n-6) *210   */
        neg += window[2] * 45ULL;                        /* a(n-8) *45    */
        neg += window[0];                                /* a(n-10) *1    */

        seq_t next = pos - neg;   /* pos >= neg for all n ≥ 11, so this is safe */

        /* slide the window */
        for (int i = 0; i < 9; ++i)
            window[i] = window[i + 1];
        window[9] = next;
    }

    return window[9];   /* a(n) is now in the last position of the window */
}
