/*
 * A227591  Number of nX3 0,1 arrays indicating 2X2 subblocks of some larger
 *          (n+1)X4 binary array having nonzero determinant.
 * https://oeis.org/A227591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 7*a(n-2) + 4*a(n-3) - 69*a(n-4) - 305*a(n-5) -
 *            518*a(n-6) - 607*a(n-7) - 271*a(n-8) - 252*a(n-9) - 12*a(n-10) +
 *            28*a(n-11) Initial values: a(1)=8, a(2)=64, a(3)=496, a(4)=3840,
 *            a(5)=29676, a(6)=229140, a(7)=1768536, a(8)=13647884,
 *            a(9)=105314460, a(10)=812639152, a(11)=6270496476
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Known initial values */
    static const seq_t init[11] = {
        8ULL,
        64ULL,
        496ULL,
        3840ULL,
        29676ULL,
        229140ULL,
        1768536ULL,
        13647884ULL,
        105314460ULL,
        812639152ULL,
        6270496476ULL
    };

    if (n <= 11) {
        return init[n - 1];
    }

    /* Buffer holding the last 11 terms:
       buf[0] = a(k-11), buf[1] = a(k-10), ..., buf[10] = a(k-1)   */
    seq_t buf[11];
    for (idx_t i = 0; i < 11; ++i) {
        buf[i] = init[i];
    }

    for (idx_t k = 12; k <= n; ++k) {
        /* Compute the next term using the linear recurrence */
        seq_t pos = buf[10] * (seq_t)7 +   /* a(k-1) */
                    buf[9] * (seq_t)7 +    /* a(k-2) */
                    buf[8] * (seq_t)4 +    /* a(k-3) */
                    buf[0] * (seq_t)28;    /* a(k-11) */

        seq_t neg = buf[7] * (seq_t)69 +   /* a(k-4) */
                    buf[6] * (seq_t)305 +  /* a(k-5) */
                    buf[5] * (seq_t)518 +  /* a(k-6) */
                    buf[4] * (seq_t)607 +  /* a(k-7) */
                    buf[3] * (seq_t)271 +  /* a(k-8) */
                    buf[2] * (seq_t)252 +  /* a(k-9) */
                    buf[1] * (seq_t)12;    /* a(k-10) */

        seq_t new_val = pos - neg;

        /* Shift buffer left by one position */
        for (int i = 0; i < 10; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[10] = new_val;
    }

    return buf[10];
}
