/*
 * A177127  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=6, k=0 and l=1.
 * https://oeis.org/A177127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) satisfies (n+1)a(n) + 2*(-3n+1)a(n-1) + (-11n+27)a(n-2) +
 *            32*(n-3)a(n-3) + 16*(-n+4)a(n-4) = 0 for n ≥ 5. Base values:
 *            a(0)=1, a(1)=6, a(2)=13, a(3)=63, a(4)=283
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation buffer */
    static seq_t *buf = NULL;
    static idx_t   cap = 0;  /* allocated size */
    static idx_t   last = 0; /* highest computed index */

    /* ensure capacity for at least n+1 elements */
    if (n + 1 > cap) {
        idx_t newcap = n + 1;
        if (newcap < 5) newcap = 5;
        if (buf == NULL) {
            buf = (seq_t *)malloc(newcap * sizeof(seq_t));
        } else {
            buf = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        }
        cap = newcap;
    }

    /* initialise base values on first use */
    if (buf[0] == 0 && last == 0) {  /* assumes seq_t zero init for unused slots */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)6;
        buf[2] = (seq_t)13;
        buf[3] = (seq_t)63;
        buf[4] = (seq_t)283;
        last = 4;
    }

    /* if already computed, return */
    if (n <= last) {
        return buf[n];
    }

    /* compute values from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* coefficients (positive integers) */
        idx_t c1 = 2 * (3 * i - 1);    /* 2*(3*i-1)  */
        idx_t c2 = 11 * i - 27;        /* 11*i-27    */
        idx_t c3 = 32 * (i - 3);       /* 32*(i-3)   */
        idx_t c4 = 16 * (i - 4);       /* 16*(i-4)   */

        seq_t term1 = (seq_t)c1 * buf[i - 1];
        seq_t term2 = (seq_t)c2 * buf[i - 2];
        seq_t term3 = (seq_t)c3 * buf[i - 3];
        seq_t term4 = (seq_t)c4 * buf[i - 4];

        seq_t sum1    = term1 + term2 + term4;
        seq_t numerator = sum1 - term3;              /* guaranteed non‑negative */

        buf[i] = numerator / (seq_t)(i + 1);
    }
    last = n;
    return buf[n];
}
