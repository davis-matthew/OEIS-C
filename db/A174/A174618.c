/*
 * A174618  For n odd a(n) = a(n-2) + a(n-3), for n even a(n) = a(n-2) +
 *          a(n-5); with a(1) = 0, a(2) = 1.
 * https://oeis.org/A174618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Recurrence a(1) = 0, a(2) = 1 For n odd : a(n) = a(n-2) + a(n-3)
 *            For n even : a(n) = a(n-2) + a(n-5) (terms with negative indices
 *            are treated as 0)
 * Range:     u8 n <= 28, u16 n <= 52, u32 n <= 98, u64 n <= 190,
 *            u128 n <= 374, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;

    /* allocate a temporary array indexed by the original n
       buf[i] holds a(i) for i = 1..n */
    seq_t *buf = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!buf)                  /* allocation failure: abort */
        abort();

    buf[1] = (seq_t)0;
    buf[2] = (seq_t)1;

    /* compute the sequence up to n iteratively */
    for (idx_t k = 3; k <= n; ++k) {
        seq_t val;
        if (k & 1U) {               /* k is odd: a(k) = a(k-2) + a(k-3) */
            seq_t p1 = (k >= 3) ? buf[k - 2] : (seq_t)0;  /* a(k-2) */
            seq_t p2 = (k >= 4) ? buf[k - 3] : (seq_t)0;  /* a(k-3) */
            val = p1 + p2;
        } else {                    /* k is even: a(k) = a(k-2) + a(k-5) */
            seq_t p1 = (k >= 3) ? buf[k - 2] : (seq_t)0;  /* a(k-2) */
            seq_t p2 = (k >= 6) ? buf[k - 5] : (seq_t)0;  /* a(k-5) */
            val = p1 + p2;
        }
        buf[k] = val;
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
