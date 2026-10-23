/*
 * A164673  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function:
   a(0)=1, a(1)=37, a(2)=1332, a(3)=47952, a(4)=1726272,
   a(5)=62145792, a(6)=2237248512.
   For n ≥ 7:
     a(n)=35*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6))
          -630*a(n-7)+p(n),
   where p(7)=1 and p(n)=0 for n>7. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *buf = NULL;
    static idx_t buf_len = 0;

    /* base values (small enough to cast safely) */
    const seq_t base[7] = {
        (seq_t)1,
        (seq_t)37,
        (seq_t)1332,
        (seq_t)47952,
        (seq_t)1726272,
        (seq_t)62145792,
        (seq_t)2237248512
    };

    /* first allocation: build up to n */
    if (buf_len == 0) {
        idx_t need = n + 1;
        buf = (seq_t *)malloc(need * sizeof(seq_t));
        if (!buf) return (seq_t)0;   /* allocation failure */

        /* initialise base terms */
        for (idx_t i = 0; i < 7 && i <= n; ++i)
            buf[i] = base[i];

        /* compute remaining terms */
        for (idx_t i = 7; i <= n; ++i) {
            seq_t sum = (seq_t)35 * buf[i-1]
                      + (seq_t)35 * buf[i-2]
                      + (seq_t)35 * buf[i-3]
                      + (seq_t)35 * buf[i-4]
                      + (seq_t)35 * buf[i-5]
                      + (seq_t)35 * buf[i-6];
            seq_t val = sum - (seq_t)630 * buf[i-7];
            if (i == 7) val += (seq_t)1;   /* p(7)=1 */
            buf[i] = val;
        }
        buf_len = need;
        return buf[n];
    }

    /* return cached value if already known */
    if (n < buf_len)
        return buf[n];

    /* enlarge cache to hold index n */
    idx_t old_len = buf_len;
    idx_t new_len = n + 1;
    seq_t *new_buf = (seq_t *)realloc(buf, new_len * sizeof(seq_t));
    if (!new_buf)
        return (seq_t)0;   /* allocation failure */
    buf = new_buf;
    buf_len = new_len;

    /* fill new entries */
    for (idx_t i = old_len; i <= n; ++i) {
        if (i < 7) {            /* set base term if still missing */
            buf[i] = base[i];
            continue;
        }
        seq_t sum = (seq_t)35 * buf[i-1]
                  + (seq_t)35 * buf[i-2]
                  + (seq_t)35 * buf[i-3]
                  + (seq_t)35 * buf[i-4]
                  + (seq_t)35 * buf[i-5]
                  + (seq_t)35 * buf[i-6];
        seq_t val = sum - (seq_t)630 * buf[i-7];
        if (i == 7) val += (seq_t)1;   /* p(7)=1 */
        buf[i] = val;
    }

    return buf[n];
}
