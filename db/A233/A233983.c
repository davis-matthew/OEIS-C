/*
 * A233983  Number of (n+1)X(2+1) 0..4 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 10, and no
 *          two adjacent values equal.
 * https://oeis.org/A233983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Empirical recurrence for A233983:
 * a(n) = 3*a(n-1) +53*a(n-2) -116*a(n-3) -892*a(n-4) +1620*a(n-5) +5804*a(n-6)
 *       -9766*a(n-7) -13812*a(n-8) +23148*a(n-9) +8224*a(n-10) -16848*a(n-11)
 *       +2560*a(n-12) +256*a(n-13)
 * Initial values (for n = 1..13):
 * a(1)=236, a(2)=1540, a(3)=9632, a(4)=62040, a(5)=393396,
 * a(6)=2520604, a(7)=16042420, a(8)=102569896, a(9)=653740236,
 * a(10)=4175880380, a(11)=26632556736, a(12)=170044378852,
 * a(13)=1084833846436
 *
 * This function memoises the sequence so that repeated calls are
 * O(1) after the first evaluation up to the requested n.
 */

#include <stdlib.h>
#include <stddef.h>  /* size_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* sequence not defined for n=0 */

    /* table of initial values */
    static const seq_t init_vals[13] = {
        236ULL, 1540ULL, 9632ULL,
        62040ULL, 393396ULL, 2520604ULL,
        16042420ULL, 102569896ULL, 653740236ULL,
        4175880380ULL, 26632556736ULL,
        170044378852ULL, 1084833846436ULL
    };

    static seq_t *table = NULL;
    static size_t len = 0;   /* number of terms stored   */
    static size_t cap = 0;   /* allocated capacity       */

    /* expand storage if necessary */
    if (len < (size_t)n) {
        size_t need = (size_t)n;
        if (cap < need) {
            size_t new_cap = cap ? cap * 2 : 16;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
            if (!tmp) return 0;       /* allocation failure */
            table = tmp;
            cap = new_cap;
        }

        /* fill in missing terms */
        while (len < (size_t)n) {
            /* insert initial values for n = 1..13 */
            if (len < 13) {
                table[len] = init_vals[len];
                ++len;
                continue;
            }

            /* compute using the recurrence */
            size_t i = len;              /* index of the term we are computing (zero‑based) */
            seq_t term = 0;
            term += table[i-1] * (seq_t)3;
            term += table[i-2] * (seq_t)53;
            term += table[i-5] * (seq_t)1620;
            term += table[i-6] * (seq_t)5804;
            term += table[i-9] * (seq_t)23148;
            term += table[i-10] * (seq_t)8224;
            term += table[i-12] * (seq_t)2560;
            term += table[i-13] * (seq_t)256;

            seq_t sub = 0;
            sub += table[i-3] * (seq_t)116;
            sub += table[i-4] * (seq_t)892;
            sub += table[i-7] * (seq_t)9766;
            sub += table[i-8] * (seq_t)13812;
            sub += table[i-11] * (seq_t)16848;

            table[len] = term - sub;
            ++len;
        }
    }

    return table[n-1];
}
