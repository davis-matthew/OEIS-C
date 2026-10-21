/*
 * A238779  Number of palindromic partitions of n with greatest part of
 *          multiplicity 2.
 * https://oeis.org/A238779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{m=1}^{⌊n/2⌋} Σ_{c=0}^{min(m-1, n-2m), n-2m-c even}
 *            p((n-2m-c)/2, m-1) where p(s,k) counts partitions of s with
 *            parts ≤ k
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of palindromic partitions of n whose greatest part
   occurs exactly twice, as defined in OEIS A238779. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* handle offset: a(1)=0 is defined by the sequence. */
    if (n <= 1) return (seq_t)0;

    idx_t maxM = n / 2;
    idx_t smax = (n >= 2) ? (n - 2) / 2 : 0;   /* maximum s needed          */
    seq_t *p = (seq_t *)malloc((smax + 1) * sizeof(seq_t));
    if (!p) return (seq_t)0;                    /* allocation failed */

    /* init p[0]=1; p[>0]=0  – partitions of 0 with parts ≤ 0 is 1  */
    p[0] = (seq_t)1;
    for (idx_t i = 1; i <= smax; ++i) p[i] = (seq_t)0;

    seq_t answer = (seq_t)0;

    for (idx_t m = 1; m <= maxM; ++m) {
        idx_t r   = n - 2 * m;                     /* remaining sum after two copies of m   */
        idx_t max_c = m - 1;                       /* centre part must be < m               */
        if (max_c > r) max_c = r;                  /* cannot exceed the remaining sum      */

        /* count contributions for this m using partitions of s with parts ≤ m‑1 */
        for (idx_t c = 0; c <= max_c; ++c) {
            if (((r - c) & 1) != 0) continue;      /* need even remainder                  */
            idx_t s = (r - c) >> 1;
            if (s <= smax) answer += p[s];
        }

        /* update p to include part size m for the next iteration */
        for (idx_t s = m; s <= smax; ++s) {
            p[s] += p[s - m];
        }
    }

    free(p);
    return answer;
}
