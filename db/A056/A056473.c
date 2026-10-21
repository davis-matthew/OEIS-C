/*
 * A056473  Number of palindromic structures using exactly four different
 *          symbols.
 * https://oeis.org/A056473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = stirling2( floor((n+1)/2) , 4 )
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses the recurrence for Stirling numbers of the second kind:
   S(0,0) = 1, S(0,k>0) = 0
   S(m,k) = k * S(m-1,k) + S(m-1,k-1)   (k = 4 constant)
   For k = 4 this reduces to:
      s4' = 4*s4 + s3
      s3' = 3*s3 + s2
      s2' = 2*s2 + s1
      s1' = 1
   The array S4 stores S(m,4) for m >= 0.  The array is filled on demand
   in O(1) amortised time per call. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* compute m = floor((n+1)/2) */
    idx_t m = (n + 1) / 2;          /* 0 <= m <= 5000 for n <= 10000 */

    /* static caching structure */
    static seq_t *S4 = NULL;        /* array of S(m,4) values   */
    static idx_t S_len = 0;         /* number of entries stored  */
    static idx_t S_cap = 0;         /* allocated capacity        */

    /* state variables holding the current Stirling values for the last computed m */
    static seq_t s1 = 0, s2 = 0, s3 = 0, s4 = 0;
    static idx_t current_m = 0;     /* the m for which s1..s4 are valid   */

    /* initialise on first call */
    if (S_len == 0) {
        /* S(0,4) = 0, we store index 0 as dummy to keep indices simple */
        S4 = (seq_t*)malloc(sizeof(seq_t));
        if (!S4) return 0;           /* out of memory – unlikely in tests */
        S4[0] = (seq_t)0;
        S_len = 1;
        S_cap = 1;
        current_m = 0;
        /* s1..s4 already 0 */
    }

    /* ensure we have values up to desired m */
    if (m >= S_len) {
        /* extend capacity if needed */
        if (m + 1 > S_cap) {
            idx_t new_cap = S_cap ? S_cap * 2 : S_len;
            if (new_cap < m + 1)
                new_cap = m + 1;
            seq_t *tmp = (seq_t*)realloc(S4, new_cap * sizeof(seq_t));
            if (!tmp) return 0;       /* allocation failure */
            S4 = tmp;
            S_cap = new_cap;
        }

        /* compute missing values, starting from current_m */
        /* current_m = S_len-1 because S4 stores indices 0..S_len-1 */
        idx_t i = S_len;             /* next m to compute */
        for (; i <= m; ++i) {
            /* recurrence for k = 4 */
            seq_t new_s4 = 4 * s4 + s3;
            seq_t new_s3 = 3 * s3 + s2;
            seq_t new_s2 = 2 * s2 + s1;
            seq_t new_s1 = (seq_t)1; /* S(i,1) = 1 for all i >= 1 */

            /* store S(i,4) */
            S4[i] = new_s4;

            /* advance state */
            s4 = new_s4;
            s3 = new_s3;
            s2 = new_s2;
            s1 = new_s1;
        }
        S_len = m + 1;
    }

    /* return the cached value */
    return S4[m];
}
