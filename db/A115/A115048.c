/*
 * A115048  Count backwards from 100 in steps modulo n.
 * https://oeis.org/A115048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = a(n-1)-(a(n-1) mod n), a(1)=100
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* static memoization of previously computed values */
    static seq_t *buf = NULL;
    static idx_t  k   = 0;    /* number of terms already stored   */
    static idx_t  cap = 0;    /* allocated capacity of the buffer */

    /* first call – allocate the buffer and set a(1) */
    if (buf == NULL) {
        cap = 8;                                   /* arbitrary small start */
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) return (seq_t)0;                  /* allocation failed */
        buf[0] = (seq_t)100;                       /* a(1) */
        k = 1;
    }

    /* need further terms up to n ? */
    if (n > k) {
        /* enlarge the buffer if necessary */
        if (n > cap) {
            idx_t new_cap = cap;
            while (new_cap < n) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;              /* allocation failed */
            buf = tmp;
            cap = new_cap;
        }

        /* compute missing terms using the recurrence */
        for (idx_t idx = k; idx < n; ++idx) {
            seq_t prev = buf[idx - 1];               /* a(idx)          */
            seq_t mod  = prev % (seq_t)(idx + 1);    /* a(idx) mod (idx+1) */
            buf[idx]   = prev - mod;                 /* a(idx+1)        */
        }
        k = n;
    }

    return buf[n - 1];
}
