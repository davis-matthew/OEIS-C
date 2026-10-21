/*
 * A064766  Fill up an infinite triangular array by rows by successively
 *          writing numbers 1, then 1 up to 2*3/2, then 1 up to 3*4/2, then 1
 *          up to 4*5/2 and so on, at the k-th step writing the numbers from 1
 *          up to the k-th triangular number. The final elements of the rows
 *          of this infinite triangle form the seq...
 * https://oeis.org/A064766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the value at index (n+1)(n+2)/2 − 1 in the concatenated
 *            blocks [1,2,…,k(k+1)/2] for k = 1,2,3,
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   tetrahedral
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage function required by the build system. */

#include <stdint.h>
#include "../utils/tetrahedral.h"

seq_t a(idx_t n)
{
    /* Position of the right‑hand element of row n (0‑based). */
    idx_t pos = ((idx_t)(n + 1) * (n + 2)) / 2 - 1;

    /* Find smallest k such that tetrahedral(k) > pos. */
    idx_t lo = 1, hi = 1;
    while (tetrahedral(hi) <= pos)
        hi <<= 1;            /* exponential increase until we pass pos */

    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (tetrahedral(mid) > pos)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t k = lo;                 /* block index (k ≥ 1) */
    idx_t prev = tetrahedral(k - 1);   /* total length before block k */
    idx_t offset = pos - prev;          /* 0‑based position within block k */

    return (seq_t)(offset + 1);     /* block numbers start at 1 */
}
