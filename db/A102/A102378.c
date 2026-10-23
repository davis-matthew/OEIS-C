/*
 * A102378  a(n) = a(n-1) + a([n/2]) + 1, a(1) = 1.
 * https://oeis.org/A102378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a([n/2]) + 1, a(1) = 1, n ≥ 1
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/*
 * Compute the n-th term of OEIS A102378.
 * The function works for any unsigned 64‑bit index n ≥ 1 and
 * uses a temporary array of length n+1 to evaluate the recurrence.
 */
seq_t a(idx_t n)
{
    /* For safety, treat n=0 as the offset before the sequence starts. */
    if (n == 0)
        return (seq_t)0;

    /* Allocate a temporary array large enough for indices 0 .. n. */
    size_t sz = (size_t)(n + 1);
    seq_t *buf = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!buf)
        return (seq_t)0;               /* allocation failed – return 0 (unused) */

    /* initialise a(0) = 0 as a convenient sentinel */
    buf[0] = (seq_t)0;
    /* base value of the sequence */
    buf[1] = (seq_t)1;

    /* compute all intermediate terms up to n   */
    for (idx_t i = 2; i <= n; ++i) {
        buf[i] = buf[i - 1] + buf[i / 2] + (seq_t)1;
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
