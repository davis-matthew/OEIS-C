/*
 * A196324  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 1,3,0,4,2
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 5*a(n-3) + 3*a(n-4) - a(n-5) + a(n-6)
 *            - 2*a(n-8) - a(n-9) with initial values: a(1)=2, a(2)=10,
 *            a(3)=27, a(4)=68, a(5)=206, a(6)=573, a(7)=1596, a(8)=4526,
 *            a(9)=12751
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial terms for n = 1..9 */
    static const seq_t init[9] = {
        2ULL, 10ULL, 27ULL, 68ULL, 206ULL,
        573ULL, 1596ULL, 4526ULL, 12751ULL
    };

    if (n == 0) /* undefined for offset 1, but return 0 for safety */
        return 0;

    /* Return directly if inside the initial range */
    if (n <= 9)
        return init[n - 1];

    /* Circular buffer holding the last 9 computed terms:
       a_hist[0] = a(k-1), a_hist[1] = a(k-2), ..., a_hist[8] = a(k-9) */
    seq_t a_hist[10];

    /* Initialize the buffer with a(9) .. a(1) */
    for (idx_t i = 0; i < 9; ++i)
        a_hist[i] = init[8 - i];

    /* Compute terms from a(10) up to a(n) */
    for (idx_t k = 10; k <= n; ++k) {
        seq_t t = a_hist[0];
        t += (seq_t)3 * a_hist[1];
        t += (seq_t)5 * a_hist[2];
        t += (seq_t)3 * a_hist[3];
        t -= a_hist[4];
        t += a_hist[5];
        t -= (seq_t)2 * a_hist[7];
        t -= a_hist[8];

        /* Shift right: a(k-1) becomes a(k-2), ... a(k-9) becomes a(k-10) */
        for (idx_t j = 8; j >= 1; --j)
            a_hist[j] = a_hist[j - 1];

        a_hist[0] = t;   /* Now a_hist[0] holds a(k) */
    }

    return a_hist[0];   /* a(n) is now in a_hist[0] */
}
