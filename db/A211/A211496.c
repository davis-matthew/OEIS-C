/*
 * A211496  Number of (n+1)X(n+1) -4..4 symmetric matrices with every 2X2
 *          subblock having sum zero and one, three or four distinct values.
 * https://oeis.org/A211496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 12*a(n-1) -36*a(n-2) -86*a(n-3)
 *            +557*a(n-4) -82*a(n-5) -2844*a(n-6) +1812*a(n-7) +6764*a(n-8)
 *            -4000*a(n-9) -7184*a(n-10) +1824*a(n-11) +1408*a(n-12)
 *            -384*a(n-13) with initial values a(1)=29, a(2)=105, a(3)=385,
 *            a(4)=1437, a(5)=5353, a(6)=19901, a(7)=73617, a(8)=271149,...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial values (index 1 .. 13) */
    static const seq_t init[14] = {
        0,                /* unused for index 0 */
        29,               /* a(1) */
        105,              /* a(2) */
        385,              /* a(3) */
        1437,             /* a(4) */
        5353,             /* a(5) */
        19901,            /* a(6) */
        73617,            /* a(7) */
        271149,           /* a(8) */
        994593,           /* a(9) */
        3635837,          /* a(10) */
        13253649,         /* a(11) */
        48203469,         /* a(12) */
        174998977        /* a(13) */
    };

    if (n <= 13) {
        return init[n];
    }

    /* buffer holds a(k-13)..a(k-1); index 0 holds a(k-13) */
    seq_t buf[13];
    for (idx_t i = 0; i < 13; ++i) {
        buf[i] = init[i + 1];
    }

    /* iterate from k=14 up to desired n */
    for (idx_t k = 14; k <= n; ++k) {
        seq_t pos = 0, neg = 0;

        /* positive coefficients */
        pos += buf[12] * 12;                  /* a(k-1)   */
        pos += buf[9]  * 557;                 /* a(k-4)   */
        pos += buf[6]  * 1812;                /* a(k-7)   */
        pos += buf[5]  * 6764;                /* a(k-8)   */
        pos += buf[2]  * 1824;                /* a(k-11)  */
        pos += buf[1]  * 1408;                /* a(k-12)  */

        /* negative coefficients */
        neg += buf[11] * 36;                  /* a(k-2)   */
        neg += buf[10] * 86;                  /* a(k-3)   */
        neg += buf[8]  * 82;                  /* a(k-5)   */
        neg += buf[7]  * 2844;                /* a(k-6)   */
        neg += buf[4]  * 4000;                /* a(k-9)   */
        neg += buf[3]  * 7184;                /* a(k-10)  */
        neg += buf[0]  * 384;                 /* a(k-13)  */

        seq_t next = pos - neg;

        /* shift buffer left, drop oldest, insert new */
        for (idx_t i = 0; i < 12; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[12] = next;
    }

    return buf[12];
}
