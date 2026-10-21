/*
 * A205110  s(k)-s(j), where (s(k),s(j)) is the least pair of numbers given by
 *          s(j)=3^j-2^j which n divides their difference.
 * https://oeis.org/A205110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive difference s(k) - s(j) (k>j≥1) with *
 *            s(t) = 3^t - 2^t that is divisible by n. * The algorithm keeps
 *            the sequence s(j) modulo n and the exact s(j) values * (using
 *            the seq_t type). For each residue we maintain the most recent *
 *            index at which it occurred; when the same res...
 * Range:     u8 n <= 7, u16 n <= 28, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>   /* for calloc */

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1).  For safety, handle 0. */
    if (n == 0)
        return 0;

    /* jmax is the index at which a repeat of a residue is guaranteed
     * to have occurred (Pigeonhole principle).  We need indices 1 … jmax. */
    idx_t jmax = n + 1;

    /* Arrays to store the exact values of s(j) and to remember the last
     * index at which each residue appeared.  Index 0 of svals is unused. */
    seq_t *svals = (seq_t *)malloc((jmax + 1) * sizeof(seq_t));
    if (!svals)
        return 0;
    idx_t *last   = (idx_t *)calloc(n, sizeof(idx_t));   /* 0 = not seen yet */
    if (!last) {
        free(svals);
        return 0;
    }

    /* Initialize for j = 1 */
    seq_t pow3 = 3;
    seq_t pow2 = 2;
    seq_t s    = pow3 - pow2;   /* 3^1 - 2^1 = 1 */
    svals[1]   = s;

    seq_t min_diff = 0;
    bool found     = false;

    for (idx_t j = 1; j <= jmax; ++j) {
        /* Current s(j) has already been computed and stored in s */
        idx_t residue = (idx_t)(s % n);

        if (last[residue] == 0) {
            /* First time this residue appears. */
            last[residue] = j;
        } else {
            /* A repeat: compute the difference with the most recent past occurrence. */
            idx_t prev    = last[residue];
            seq_t diff    = s - svals[prev];
            if (!found || diff < min_diff) {
                min_diff = diff;
                found    = true;
            }
            /* Update the most recent occurrence. */
            last[residue] = j;
        }

        /* Prepare the next term (unless we have reached the last needed index). */
        if (j == jmax)
            break;
        pow3 = pow3 * 3;
        pow2 = pow2 * 2;
        s    = pow3 - pow2;
        svals[j + 1] = s;
    }

    free(last);
    free(svals);
    return min_diff;
}
