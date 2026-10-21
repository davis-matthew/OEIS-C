/*
 * A234931  Number of tilings of a 5 X n rectangle using n pentominoes of
 *          shapes F, U, N.
 * https://oeis.org/A234931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 29, u32 n <= 48, u64 n <= 87, u128 n <= 165,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the number of tilings of a 5×n rectangle by n pentominoes
 * of shapes F, U, N.  It satisfies the linear recurrence
 *
 *   a(n) =  2*a(n‑1) +      a(n‑4) - 6*a(n‑5) + 2*a(n‑6) - 2*a(n‑7)
 *           + 8*a(n‑8) + 2*a(n‑9) + 7*a(n‑10) -14*a(n‑11) - 9*a(n‑12)
 *           - 8*a(n‑13) + 7*a(n‑14) + 4*a(n‑15) + 26*a(n‑16) + 8*a(n‑17)
 *           - 6*a(n‑18) -28*a(n‑20) - 8*a(n‑22)            (n ≥ 22)
 *
 * Initial values for 0 ≤ n ≤ 21:
 *   a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=0, a(6)=0,
 *   a(7)=0, a(8)=4, a(9)=0, a(10)=8, a(11)=0, a(12)=16, a(13)=0,
 *   a(14)=40, a(15)=0, a(16)=64, a(17)=16, a(18)=200, a(19)=96,
 *   a(20)=504, a(21)=464.
 */

#include <stddef.h>
#include "../utils/mul_small.h"

static const int coeff[23] = {
    0,   /* unused index 0 */
    2,   /* c[1]  */
    0,   /* c[2]  */
    0,   /* c[3]  */
    1,   /* c[4]  */
   -6,   /* c[5]  */
    2,   /* c[6]  */
   -2,   /* c[7]  */
    8,   /* c[8]  */
    2,   /* c[9]  */
    7,   /* c[10] */
   -14,  /* c[11] */
   -9,   /* c[12] */
   -8,   /* c[13] */
    7,   /* c[14] */
    4,   /* c[15] */
   26,   /* c[16] */
    8,   /* c[17] */
   -6,   /* c[18] */
    0,   /* c[19] */
   -28,  /* c[20] */
    0,   /* c[21] */
   -8    /* c[22] */
};

static const unsigned long long init_vals_raw[22] = {
    1ULL, 0ULL, 0ULL, 0ULL, 2ULL, 0ULL, 0ULL, 0ULL,
    4ULL, 0ULL, 8ULL, 0ULL, 16ULL, 0ULL, 40ULL, 0ULL,
    64ULL, 16ULL, 200ULL, 96ULL, 504ULL, 464ULL
};

seq_t a(idx_t n)
{
    if (n <= 21)
        return (seq_t)init_vals_raw[n];

    /* buffer[0] holds a(n-1), buffer[21] holds a(n-22) */
    seq_t buffer[22];
    for (size_t i = 0; i < 22; ++i)
        buffer[i] = (seq_t)init_vals_raw[21 - i];

    for (idx_t cur = 22; cur <= n; ++cur)
    {
        seq_t pos_sum = 0, neg_sum = 0;
        for (int j = 1; j <= 22; ++j)
        {
            int c = coeff[j];
            if (c != 0)
            {
                seq_t mult = mul_small(buffer[j - 1], (unsigned int)(c > 0 ? c : -c));
                if (c > 0)
                    pos_sum += mult;
                else
                    neg_sum += mult;
            }
        }
        seq_t new_val = pos_sum - neg_sum;

        /* shift buffer right, inserting new_val at buffer[0] */
        for (int i = 21; i >= 1; --i)
            buffer[i] = buffer[i - 1];
        buffer[0] = new_val;
    }

    return buffer[0];
}
