/*
 * A221633  Equals one maps: number of 2 X n binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          horizontal, diagonal and antidiagonal neighbors in a random 0..3 2
 *          X n array.
 * https://oeis.org/A221633
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

/* Recurrence: a(1)=1, a(2)=8, a(3)=37, a(4)=176, a(5)=787, a(6)=3480,
 * a(7)=14865, a(8)=62064, a(9)=254463, a(10)=1032248.
 * For n>10:
 *   a(n)=6*a(n-1) - 3*a(n-2) - 32*a(n-3) + 45*a(n-4) + 30*a(n-5)
 *   - 77*a(n-6) + 12*a(n-7) + 36*a(n-8) - 16*a(n-9)          */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial values a(1)..a(10) */
    static const seq_t init[10] = {
        1ULL,        /* a(1) */
        8ULL,        /* a(2) */
        37ULL,       /* a(3) */
        176ULL,      /* a(4) */
        787ULL,      /* a(5) */
        3480ULL,     /* a(6) */
        14865ULL,    /* a(7) */
        62064ULL,    /* a(8) */
        254463ULL,   /* a(9) */
        1032248ULL   /* a(10) */
    };

    if (n <= 10) {
        return init[n - 1];
    }

    /* Window of the last nine terms: a1=a(n-9), ..., a9=a(n-1) */
    seq_t a1 = init[1]; /* a(2) when n=11 */
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];
    seq_t a6 = init[6];
    seq_t a7 = init[7];
    seq_t a8 = init[8];
    seq_t a9 = init[9]; /* a(10) when n=11 */

    for (idx_t i = 11; i <= n; ++i) {
        seq_t new_val = 6 * a9
                      - 3 * a8
                      - 32 * a7
                      + 45 * a6
                      + 30 * a5
                      - 77 * a4
                      + 12 * a3
                      + 36 * a2
                      - 16 * a1;

        /* shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = a8;
        a8 = a9;
        a9 = new_val;
    }

    return a9;
}
