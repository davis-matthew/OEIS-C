/*
 * A200060  Number of -n..n arrays x(0..5) of 6 elements with zero sum and
 *          elements alternately strictly increasing and strictly decreasing.
 * https://oeis.org/A200060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + 2*a(n-5) - a(n-6) - a(n-7)
 *            + 2*a(n-8) - a(n-10) - 2*a(n-11) + 3*a(n-12) - a(n-13)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 67, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values a(1) through a(13) are hard‑coded according to OEIS. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* first 13 terms of the sequence */
    static const seq_t init[13] = {
        10ULL,    178ULL,   1098ULL,   4172ULL,   11962ULL,
        28554ULL, 59910ULL, 114232ULL, 202314ULL, 337902ULL,
        538054ULL,823496ULL,1218978ULL
    };

    if (n <= 13) {
        if (n == 0) return 0;   /* undefined for n=0, return 0 */
        return init[n - 1];
    }

    /* buffer of the last 13 values; buf[0] = a(i‑13), buf[12] = a(i‑1) */
    seq_t buf[13];
    for (size_t i = 0; i < 13; i++) buf[i] = init[i];

    for (idx_t i = 14; i <= n; i++) {
        /* compute  a(i)  only with unsigned arithmetic:
           positive part           : 3*a(i-1) + 2*a(i-5) + 2*a(i-8) + 3*a(i-12)
           negative part (to subtract): 2*a(i-2) + a(i-3) + a(i-6) + a(i-7)
                                          + a(i-10) + 2*a(i-11) + a(i-13)   */
        __int128 pos = (__int128)buf[12] * 3
                      + (__int128)buf[8]  * 2
                      + (__int128)buf[5]  * 2
                      + (__int128)buf[1]  * 3;
        __int128 neg = (__int128)buf[11] * 2
                      + (__int128)buf[10]
                      + (__int128)buf[7]
                      + (__int128)buf[6]
                      + (__int128)buf[3]
                      + (__int128)buf[2] * 2
                      + (__int128)buf[0];
        __int128 val = pos - neg;
        seq_t next = (seq_t)val;

        /* slide the window one step forward */
        for (size_t k = 0; k < 12; k++) buf[k] = buf[k + 1];
        buf[12] = next;
    }

    return buf[12];
}
