/*
 * A049069  Array T read by antidiagonals: T(k,n) = k*n*2^(n-1) + 1, n >= 0, k
 *          >= 1.
 * https://oeis.org/A049069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n‑th term of the array read by antidiagonals of the
 *            table T(k,n) = k*n*2^(n-1)+1, with the values within each
 *            antidiagonal sorted in ascending order
 * Range:     u8 n <= 33, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the diagonal m (where m starts at 0) that contains the
       index n.  Each diagonal m has m+1 elements, so the cumulative
       count of terms up to diagonal m is (m+1)(m+2)/2. */
    idx_t m = 0;
    idx_t cum_before = 0;            /* terms before diagonal m */
    while (1) {
        idx_t next = cum_before + (m + 1);  /* end of current diagonal */
        if (n < next)
            break;
        cum_before = next;
        ++m;
    }

    idx_t offset = n - cum_before;          /* position within diagonal */
    idx_t k = (m + 1) - offset;             /* column index in T(k, n) */
    idx_t nindex = offset;                  /* row index in T(k, n) */

    if (nindex == 0)                         /* T(k,0) = 1 */
        return (seq_t)1;

    seq_t pow_val = pow2(nindex - 1);        /* 2^(nindex-1) */
    seq_t val = ((seq_t)k) * ((seq_t)nindex) * pow_val + (seq_t)1;
    return val;
}
