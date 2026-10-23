/*
 * A170079  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170079:
   a(0) = 1;
   for n ≥ 1 let S(n) = ∑_{k=1}^{36} a(n-k)   (a(m)=0 if m<0).
   Then
       a(n) = 20·S(n) - 210·a(n-37) + C(n),
   where  C(n) = 2 for 1 ≤ n ≤ 36,
             C(37) = 1,
             C(n) = 0 for n > 37. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf   = NULL;     /* cached values a[0..cap-1]   */
    static size_t cap   = 0;        /* allocated capacity   */
    static size_t used  = 0;        /* highest index computed (a[0..used-1] exist) */

    /* ensure capacity for at least n+1 entries */
    if (cap <= n) {
        size_t newcap = cap == 0 ? 1 : cap << 1;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);  /* out of memory */
        buf = tmp;
        cap = newcap;
    }

    /* compute missing terms up to n */
    if (used == 0) {               /* initialize a(0) */
        buf[0] = (seq_t)1;
        used = 1;
    }
    for (idx_t i = used; i <= n; ++i) {
        seq_t sumPrev = (seq_t)0;
        /* sum of the previous up to 36 terms */
        for (idx_t k = 1; k <= 36 && i >= k; ++k) {
            sumPrev += buf[i - k];
        }
        seq_t val = (seq_t)20 * sumPrev;          /* 20 * sumPrev */
        if (i >= 37) {
            val -= (seq_t)210 * buf[i - 37];      /* -210 * a(n-37) */
        }
        if (i <= 36) {
            val += (seq_t)2;                   /* +2 for 1 ≤ n ≤ 36  */
        } else if (i == 37) {
            val += (seq_t)1;                   /* +1 for n = 37       */
        }
        buf[i] = val;
    }
    used = n + 1;
    return buf[n];
}
