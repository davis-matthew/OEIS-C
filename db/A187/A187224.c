/*
 * A187224  Rank transform of the sequence floor(3*n/2).
 * https://oeis.org/A187224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rank transform of floor(3*n/2) computed recursively.
   a(1)=1, a(n)=floor(3*n/2).  For each n>1,
   h(n) counts r(i) with a(n-1)<=r(i)<a(n), then
   r(n)=r(n-1)+h(n)+1.  The returned value is r(n). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n<1, return 0 as guard */
    if (n == 1) return 1;

    /* Allocate array for r[1..n] */
    seq_t *r = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!r) return 0;         /* allocation failure */

    r[1] = 1;
    size_t cursor = 1;        /* first index of r that is >= current a_prev */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a_curr = floor(3*i/2) */
        seq_t a_curr = (seq_t)((seq_t)i * 3 / 2);

        /* Count r values in [a_prev, a_curr) */
        seq_t count = 0;
        /* Only r[1..i-1] are defined */
        while (cursor <= (size_t)(i - 1) && r[cursor] < a_curr)
        {
            ++count;
            ++cursor;
        }

        /* r[i] = r[i-1] + count + 1 */
        r[i] = r[i - 1] + count + 1;
    }

    seq_t result = r[n];
    free(r);
    return result;
}
