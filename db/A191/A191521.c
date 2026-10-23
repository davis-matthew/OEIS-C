/*
 * A191521  Triangle read by rows: T(n,k) is the number of left factors of
 *          Dyck paths of length n that have k valleys (i.e., a (1,-1)-step
 *          followed by a (1,1)-step).
 * https://oeis.org/A191521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened triangle T(n,k) with T(n,k) = C((n+1)/2,
 *            k+1)*C((n-1)/2, k) if n is odd, = C(n/2, k)*C(n/2,
 *            k+1)*(n/2+1)/(n/2) if n is even
 * Range:     u8 n <= 38, u16 n <= 114, u32 n <= 351, u64 n <= 1241,
 *            u128 n <= 4588, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility function: binom(a,b) returns \binom{a}{b},
   defined in the utilities. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row N such that the cumulative number of elements
       of all rows before row N is <= n, and the cumulative
       number including row N is > n.  Then k = n - cumulative
       is the column index within that row. */

    idx_t N = 0;
    idx_t cumulative = 0;            /* elements before row N */

    while (1) {
        seq_t len = (N == 0) ? (seq_t)1 : (seq_t)((N + 1) / 2);
        if (cumulative + (idx_t)len > n)
            break;
        cumulative += (idx_t)len;
        ++N;
    }

    idx_t k = n - cumulative;        /* column index in row N */

    /* Special case: row 0 has a single entry = 1. */
    if (N == 0)
        return (seq_t)1;

    if ((N & 1) == 0) { /* even N: N = 2m */
        idx_t m = N >> 1; /* m = N/2 */
        seq_t c1 = binom(m, k);      /* C(m, k)   */
        seq_t c2 = binom(m, k + 1);  /* C(m, k+1) */
        seq_t prod = c1 * c2;        /* product */
        prod = prod / (idx_t)m;      /* divide by m  (exact) */
        prod = prod * (seq_t)(m + 1);/* multiply by (m+1) */
        return prod;
    } else { /* odd N: N = 2m+1, let L = (N+1)/2 = m+1 */
        idx_t L = (N + 1) >> 1; /* L = (N+1)/2 */
        seq_t c1 = binom(L, k + 1);   /* C(L, k+1)   */
        seq_t c2 = binom(L - 1, k);   /* C(L-1, k)   */
        return c1 * c2;
    }
}
