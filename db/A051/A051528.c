/*
 * A051528  Molien series for group G_{1,2} of order 384.
 * https://oeis.org/A051528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1, a(1)=0, a(2)=0, a(3)=0, a(4)=4, a(5)=2, a(6)=3,
 *            a(7)=2, a(8)=11, a(9)=7, a(10)=11, a(11)=9, a(12)=25, a(13)=18,
 *            a(14)=27, a(15)=23, for n≥16 : a(n) = a(n-1) + 2*a(n-4) -
 *            2*a(n-5) + a(n-6) - a(n-7) - a(n-8) + a(n-9) - 2*a(n-10) +
 *            2*a(n-11) + a(n-14) - a(n-15)
 * Range:     u8 n <= 31, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values for 0 ≤ n ≤ 15 */
    static const seq_t init[16] = {
        1, 0, 0, 0,
        4, 2, 3, 2,
        11, 7, 11, 9,
        25, 18, 27, 23
    };

    static seq_t *buf = NULL;   /* cache of computed terms */
    static size_t len = 0;      /* number of elements already stored (len = highest_index+1) */
    static size_t cap = 0;      /* allocated capacity of buf */

    /* Ensure capacity is sufficient for index n. */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failed */
        buf = tmp;
        cap = newcap;
    }

    /* Compute terms up to n if not already done. */
    if ((size_t)n >= len) {
        for (idx_t i = len; i <= n; ++i) {
            size_t idx = (size_t)i;
            if (i < 16) {
                buf[idx] = init[i];
            } else {
                /* Construct the recurrence in two signed sums to avoid
                   intermediate underflow in unsigned arithmetic.          */
                seq_t pos = buf[idx - 1];                /* a(n-1) */
                pos += (seq_t)2 * buf[idx - 4];          /* + 2*a(n-4) */
                pos += buf[idx - 6];                     /* + a(n-6) */
                pos += buf[idx - 9];                     /* + a(n-9) */
                pos += (seq_t)2 * buf[idx - 11];         /* + 2*a(n-11) */
                pos += buf[idx - 14];                    /* + a(n-14) */

                seq_t neg = (seq_t)2 * buf[idx - 5];     /* - 2*a(n-5) */
                neg += buf[idx - 7];                     /* - a(n-7) */
                neg += buf[idx - 8];                     /* - a(n-8) */
                neg += (seq_t)2 * buf[idx - 10];         /* - 2*a(n-10) */
                neg += buf[idx - 15];                    /* - a(n-15) */

                buf[idx] = pos - neg;
            }
        }
        len = (size_t)n + 1;
    }

    return buf[(size_t)n];
}
