/*
 * A188460  Diagonal sums of number triangle A119308.
 * https://oeis.org/A188460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): (n+5)*(n+1)*a(n) - (2*n+5)*(n+2)*a(n-1)
 *                - (n+2)*(n+1)*a(n-2) - (n+1)*(2*n+1)*a(n-3)
 *                + (n-2)*(n+2)*a(n-4) = 0
 * Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=9.  */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *memo = NULL;      /* array of seq_t */
    static idx_t memo_len = 0;      /* current allocated size */

    /* ----------------------------------------------------------- */
    /* Helper to allocate or re‑allocate the cache                 */
    /* ----------------------------------------------------------- */
    if (memo_len == 0) {             /* first allocation */
#if defined(__cplusplus)
        memo = new seq_t[n + 1];     /* value‑initialised objects (C++) */
#else
        memo = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
#endif
        if (!memo) return 0;        /* allocation failed */
        memo_len = n + 1;
    } else if (n + 1 > memo_len) {   /* need more space */
#if defined(__cplusplus)
        seq_t *tmp = new seq_t[n + 1];
        for (idx_t i = 0; i < memo_len; ++i)
            tmp[i] = memo[i];
        delete[] memo;
#else
        seq_t *tmp = (seq_t *)realloc(memo, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;        /* reallocation failed */
#endif
        memo = tmp;
        memo_len = n + 1;
    }

    /* ----------------------------------------------------------- */
    /* Initialise the known first terms once                           */
    /* ----------------------------------------------------------- */
    if (memo_len >= 1) memo[0] = (seq_t)1;
    if (memo_len >= 2) memo[1] = (seq_t)2;
    if (memo_len >= 3) memo[2] = (seq_t)4;
    if (memo_len >= 4) memo[3] = (seq_t)9;

    /* ----------------------------------------------------------- */
    /* Compute missing terms using the recurrence                   */
    /* ----------------------------------------------------------- */
    if (n < 4)
        return memo[n];   /* first four values handled above */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t a1 = memo[i - 1];
        seq_t a2 = memo[i - 2];
        seq_t a3 = memo[i - 3];
        seq_t a4 = memo[i - 4];

        seq_t term1 = ((seq_t)(2 * i + 5) * (seq_t)(i + 2)) * a1;
        seq_t term2 = ((seq_t)(i + 2) * (seq_t)(i + 1)) * a2;
        seq_t term3 = ((seq_t)(i + 1) * (seq_t)(2 * i + 1)) * a3;
        seq_t term4 = ((seq_t)(i - 2) * (seq_t)(i + 2)) * a4;

        seq_t num   = term1 + term2 + term3 - term4;
        seq_t denom = ((seq_t)(i + 5) * (seq_t)(i + 1));

        memo[i] = num / denom;   /* the recurrence guarantees integer division */
    }

    return memo[n];
}
